/*

  This is an example tool that converts OSM data to a spatialite database using
  the OGR library.

  This version creates multipolygons and reads the input file twice to do that.

*/

/*

Copyright 2012 Jochen Topf <jochen@topf.org> and others (see README).

This file is part of Osmium (https://github.com/joto/osmium).

Osmium is free software: you can redistribute it and/or modify it under the
terms of the GNU Lesser General Public License or (at your option) the GNU
General Public License as published by the Free Software Foundation, either
version 3 of the Licenses, or (at your option) any later version.

Osmium is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU Lesser General Public License and the GNU
General Public License for more details.

You should have received a copy of the Licenses along with Osmium. If not, see
<http://www.gnu.org/licenses/>.

*/

#include <cstdlib>

#include <ogrsf_frmts.h>

#define OSMIUM_WITH_PBF_INPUT
#define OSMIUM_WITH_XML_INPUT

#include <osmium.hpp>
#include <osmium/storage/byid/sparse_table.hpp>
#include <osmium/storage/byid/mmap_file.hpp>
#include <osmium/handler/coordinates_for_ways.hpp>
#include <osmium/relations/multipolygon_assembler.hpp>
#include <osmium/geometry/multipolygon.hpp>
#include <osmium/geometry/ogr.hpp>

typedef Osmium::Storage::ById::SparseTable<Osmium::OSM::Position> storage_sparsetable_t;
typedef Osmium::Storage::ById::MmapFile<Osmium::OSM::Position> storage_mmap_t;
typedef Osmium::Handler::CoordinatesForWays<storage_sparsetable_t, storage_mmap_t> cfw_handler_t;

/* ================================================== */

class MyOGRHandlerPass2 : public Osmium::Handler::Base {

    OGRDataSource* m_data_source;
    OGRLayer* m_layer_mp;

public:

    MyOGRHandlerPass2() {
        OGRRegisterAll();

        const char* driver_name = "SQLite";
        OGRSFDriver* driver = OGRSFDriverRegistrar::GetRegistrar()->GetDriverByName(driver_name);
        if (driver == NULL) {
            std::cerr << driver_name << " driver not available.\n";
            exit(1);
        }

        CPLSetConfigOption("OGR_SQLITE_SYNCHRONOUS", "FALSE");
        const char* options[] = { "SPATIALITE=TRUE", NULL };
        m_data_source = driver->CreateDataSource("ogr_out.sqlite", const_cast<char**>(options));
        if (m_data_source == NULL) {
            std::cerr << "Creation of output file failed.\n";
            exit(1);
        }

        OGRSpatialReference sparef;
        sparef.SetWellKnownGeogCS("WGS84");
        m_layer_mp = m_data_source->CreateLayer("areas", &sparef, wkbMultiPolygon, NULL);
        if (m_layer_mp == NULL) {
            std::cerr << "Layer creation failed.\n";
            exit(1);
        }

        OGRFieldDefn layer_mp_field_id("id", OFTInteger);
        layer_mp_field_id.SetWidth(10);

        if (m_layer_mp->CreateField(&layer_mp_field_id) != OGRERR_NONE ) {
            std::cerr << "Creating id field failed.\n";
            exit(1);
        }
    }

    ~MyOGRHandlerPass2() {
        OGRDataSource::DestroyDataSource(m_data_source);
    }

    void area(Osmium::OSM::Area* area) {
        const char* building = area->tags().get_tag_by_key("building");
        if (building) {
            try {
                Osmium::Geometry::MultiPolygon mp(*area);

                OGRFeature* feature = OGRFeature::CreateFeature(m_layer_mp->GetLayerDefn());
                OGRMultiPolygon* ogrmp = Osmium::Geometry::create_ogr_geometry(mp);
                feature->SetGeometry(ogrmp);
                feature->SetField("id", area->id());

                if (m_layer_mp->CreateFeature(feature) != OGRERR_NONE) {
                    std::cerr << "Failed to create feature.\n";
                    exit(1);
                }

                OGRFeature::DestroyFeature(feature);
                delete ogrmp;
            } catch (Osmium::Exception::IllegalGeometry) {
                std::cerr << "Ignoring illegal geometry for multipolygon " << area->id() << ".\n";
            }
        }
    }

};

MyOGRHandlerPass2* hpass2;

/* ================================================== */

void cbmp(Osmium::OSM::Area* area) {
    std::cout << "cbmp\n";
    hpass2->area(area);
}

template <class T>
class PrepHandler : public Osmium::Handler::Base {

    storage_sparsetable_t store_pos;
    storage_mmap_t store_neg;
    cfw_handler_t* handler_cfw;
    T& m_handler_multipolygon;

public:

    PrepHandler(T& handler_multipolygon) :
        m_handler_multipolygon(handler_multipolygon) {
        handler_cfw = new cfw_handler_t(store_pos, store_neg);
    }

    ~PrepHandler() {
        delete handler_cfw;
    }

    void init(Osmium::OSM::Meta& meta) {
        handler_cfw->init(meta);
    }

    void node(const shared_ptr<Osmium::OSM::Node const>& node) {
        handler_cfw->node(node);
    }

    void after_nodes() {
        handler_cfw->after_nodes();
    }

    void before_ways() {
        m_handler_multipolygon.before_ways();
    }

    void way(const shared_ptr<Osmium::OSM::Way>& way) {
        handler_cfw->way(way);
        m_handler_multipolygon.way(way);
    }

    void after_ways() {
        m_handler_multipolygon.after_ways();
    }

};

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " OSMFILE" << std::endl;
        exit(1);
    }

    Osmium::OSMFile infile(argv[1]);

    bool attempt_repair = true;

    MyOGRHandlerPass2 mphandler;
    hpass2 = &mphandler;
    Osmium::Relations::MultiPolygonAssembler<MyOGRHandlerPass2> assembler(mphandler, attempt_repair, cbmp);
    PrepHandler<Osmium::Relations::MultiPolygonAssembler<MyOGRHandlerPass2>::HandlerPass2> prephandler(assembler.handler_pass2());

    std::cerr << "First pass...\n";
    Osmium::Input::read(infile, assembler.handler_pass1());
    std::cerr << "Second pass...\n";
    Osmium::Input::read(infile, prephandler);
}

