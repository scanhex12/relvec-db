#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "../persistent_kv/kv_storage.h"

#include <random>
#include <iostream>

#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#include <atomic>
#include <random>


void error_exit() {
    fprintf(stderr, "Error: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
}

void* mount_shared_memory(const std::string& name) {
    int fd = shm_open(name.c_str(), O_CREAT | O_RDWR, 0777);
    if (fd < 0) {
        error_exit();
    }
    if (ftruncate(fd, 1024) != 0) {
        error_exit();
    }

    void* ret = mmap(0, 1024, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (ret == (void*)MAP_FAILED) {
        error_exit();
    }

    return ret;
}

std::string generate_key(std::default_random_engine& e) {
    std::uniform_int_distribution<> d(0, 99999);
    return std::to_string(d(e));
}

std::string generate_val(std::default_random_engine& e) {
    std::uniform_int_distribution<char> d(0, 255);
    std::uniform_int_distribution<> ds(2, 100);

    std::string ret;
    int len = ds(e);
    for (int i = 0; i < len; i++) {
        ret += d(e);
    }

    return ret;
}

void run(int seed, std::atomic<int>* counter, NKV::KVStorage* db) {
    std::default_random_engine e(seed);
    std::uniform_int_distribution<> d(0, 5);
    while (true) {
        std::string key = generate_key(e);
        if (d(e) == 0) {
            db->erase(key);
        } else {
            std::string value = generate_val(e);
            db->set(key, value);
        }
        ++*counter;
    }
}

void run_std(int seed, int count,
             std::unordered_map<std::string, std::string>* values) {
    std::default_random_engine e(seed);
    std::uniform_int_distribution<> d(0, 5);
    for (int i = 0; i < count; i++) {
        std::string key = generate_key(e);
        if (d(e) == 0) {
            values->erase(key);
        } else {
            std::string value = generate_val(e);
            (*values)[key] = value;
        }
    }
}

int runTestCase(int seed, int runtime_usec) {
    std::random_device rd;
    std::uniform_int_distribution<int> dist(0, 1000000000);
    std::string filename = "test_db" + std::to_string(dist(rd));
    std::string shm_name = "/wal_shared_segment" + std::to_string(dist(rd));

    shm_unlink(shm_name.c_str());
    void* parent_ptr = mount_shared_memory(shm_name);
    std::atomic<int>* parent_counter = new(parent_ptr)std::atomic<int>();

    pid_t child_pid = fork();

    if (child_pid == -1) {
        error_exit();
    }

    if (!child_pid) {
        NKV::KVStorage db(filename.c_str());
        void* child_ptr = mount_shared_memory(shm_name);
        std::atomic<int>* child_counter = reinterpret_cast<std::atomic<int>*>(child_ptr);
        run(seed, child_counter, &db);
        return 0;
    }

    usleep(runtime_usec);

    kill(child_pid, SIGKILL);
    int wstatus;
    waitpid(child_pid, &wstatus, 0);

    int num = parent_counter->load();
    shm_unlink(shm_name.c_str());

    if (1e6 * num / (double)runtime_usec < 50) {
        fprintf(stderr, "Low throughput, only %d writes in %d usec, or %.2f rps",
            num, runtime_usec, (double) (1e6 * num / runtime_usec));
        return 1;
    }

    std::unordered_map<std::string, std::string> mine;
    std::unordered_map<std::string, std::string> mine1;

    run_std(seed, num, &mine);
    run_std(seed, num + 1, &mine1);

    NKV::KVStorage recovered(filename.c_str());

    if (unlink(filename.c_str())) {
        fprintf(stderr, "Unable to delete %s: %s\n", filename.c_str(), strerror(errno));
    }

    if (recovered.values() == mine || recovered.values() == mine1) {
        return 0;
    } else {
        printf("%d\n", num);
        printf("Mismatch\n");
        return 1;
    }
}

TEST_CASE("persistent_kv") {
    REQUIRE(runTestCase(12, 10000) == 0);
    REQUIRE(runTestCase(17, 10000) == 0);
    REQUIRE(runTestCase(18, 10000) == 0);
    REQUIRE(runTestCase(20, 10000) == 0);
    REQUIRE(runTestCase(22, 100000) == 0);
    REQUIRE(runTestCase(23, 100000) == 0);
    REQUIRE(runTestCase(24, 100000) == 0);
    REQUIRE(runTestCase(25, 100000) == 0);
    REQUIRE(runTestCase(26, 1000000) == 0);
    REQUIRE(runTestCase(27, 1000000) == 0);
    REQUIRE(runTestCase(28, 1000000) == 0);
}