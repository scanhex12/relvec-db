#pragma once

#include <map>
#include <vector>
#include <iostream>

namespace shdb {

    template<class Key, class Value>
    class ClockCache {
        // Your code goes here.
    public:
        ClockCache(const std::vector<Value> &values) :
                Values_(values) {
            Capacity_ = values.size();
            Clock_ = std::vector<int>(Capacity_);
            Keys_.resize(Capacity_);
            std::cout << Capacity_ << '\n';
        }

        std::pair<bool, int> find(Key key) {
            for (size_t i = 0; i < Keys_.size(); ++i) {
                if (Keys_[i] == key) {
                    return {1, i};
                }
            }
            return {0, 0};
        }

        int put(Key key) {
            if ((int) Clock_.size() >= Capacity_) {
                while (Clock_[cur_iter] == 1) {
                    Clock_[cur_iter] = 0;
                    cur_iter = (cur_iter + 1) % Clock_.size();
                }
                Clock_[cur_iter] = 1;
            }
            Keys_[cur_iter] = key;
            int prev_iter = cur_iter;
            cur_iter = (cur_iter + 1) % Clock_.size();
            return prev_iter;
        }

        void lock(Key key) {
            for (size_t i = 0; i < Keys_.size(); ++i) {
                if (Keys_[i] == key) {
                    Clock_[i] = 1;
                }
            }
        }

        void unlock(Key key) {
            for (size_t i = 0; i < Keys_.size(); ++i) {
                if (Keys_[i] == key) {
                    Clock_[i] = 0;
                }
            }
        }

    private:
        std::map<Key, std::pair<Value, int>> Cache_;
        int Capacity_;
        std::vector<int> Clock_;
        std::vector<Key> Keys_;
        std::vector<Value> Values_;

        int cur_iter = 0;
    };

}    // namespace shdb

