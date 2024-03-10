rm -rf build && \
mkdir build && \
cd build && \
cmake .. && \
make && \
cd tests && \
./tests_serialize && \
cd ../master/tests && \
./tests_kv && \
cd ../lib/tests && \
./tests_sql && \
cd ../../..