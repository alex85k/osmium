SET BOOST=r:/libs/boost
SET PREFIX=r:/libs
cmake .. -G "NMake Makefiles" -DBOOST_ROOT=%BOOST% -DBUILD_WITH_STATIC_BOOST=ON -DCMAKE_BUILD_TYPE=Release -DSEARCH_PREFIX=%PREFIX% -DCMAKE_INSTALL_PREFIX=%PREFIX%/osimum
