/*
 * This file is open source software, licensed to you under the terms
 * of the Apache License, Version 2.0 (the "License").  See the NOTICE file
 * distributed with this work for additional information regarding copyright
 * ownership.  You may not use this file except in compliance with the License.
 *
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */
/*
 * Copyright (C) 2018 ScyllaDB.
 */

#include <seastar/testing/test_case.hh>

#include <seastar/core/reactor.hh>
#include <seastar/core/seastar.hh>
#include <seastar/core/thread.hh>

using namespace seastar;

SEASTAR_TEST_CASE(access_flags_test) {
    access_flags flags = access_flags::read | access_flags::write  | access_flags::execute;
    BOOST_REQUIRE(std::underlying_type_t<open_flags>(flags) ==
                  (std::underlying_type_t<open_flags>(access_flags::read) |
                   std::underlying_type_t<open_flags>(access_flags::write) |
                   std::underlying_type_t<open_flags>(access_flags::execute)));
    return make_ready_future<>();
}

SEASTAR_TEST_CASE(file_exists_test) {
    return seastar::async([] {
        sstring filename = "testfile.tmp";
        auto f = open_file_dma(filename, open_flags::rw | open_flags::create).get0();
        f.close().get();
        auto exists = file_exists(filename).get0();
        BOOST_REQUIRE(exists);
        remove_file(filename).get();
        exists = file_exists(filename).get0();
        BOOST_REQUIRE(!exists);
    });
}

SEASTAR_TEST_CASE(file_access_test) {
    return seastar::async([] {
        sstring filename = "testfile.tmp";
        auto f = open_file_dma(filename, open_flags::rw | open_flags::create).get0();
        f.close().get();
        auto is_accessible = file_accessible(filename, access_flags::read | access_flags::write).get0();
        BOOST_REQUIRE(is_accessible);
        remove_file(filename).get();
    });
}
