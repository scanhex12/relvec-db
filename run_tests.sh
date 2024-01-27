rm -rf build && \
mkdir build && \
cd build && \
cmake .. && \
make && \
cd tests && \
./tests && \
cd ../master/tests && \
./tests_kv && \
cd ../../..