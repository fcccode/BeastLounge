//
// Copyright (c) 2018-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/boostorg/beast
//

#include <nlohmann/json.hpp>
#include <boost/beast/_experimental/unit_test/suite.hpp>

#include <boost/beast/_experimental/json/basic_parser.hpp>
#include <boost/beast/_experimental/json/parse_file.hpp>

namespace boost {
namespace beast {

class nlohmann_parser : public boost::beast::json::basic_parser
{
    nlohmann::json root_;
    std::vector<nlohmann::json*> stack_;
    std::string s_;

public:
    nlohmann_parser()
    {
    }

    nlohmann::json
    release()
    {
        reset();
        return std::move(root_);
    }

    nlohmann::json&
    get() noexcept
    {
        return root_;
    }

    nlohmann::json const&
    get() const noexcept
    {
        return root_;
    }

private:
    template<class Value>
    void
    assign(Value&& v)
    {
        if(stack_.back()->is_object())
        {
            BOOST_ASSERT(! key().empty());
            (*stack_.back())[key().to_string()] = v;
        }
        else if(stack_.back()->is_array())
        {
            BOOST_ASSERT(key().empty());
            stack_.back()->push_back(v);
        }
        else
        {
            BOOST_ASSERT(stack_.back()->is_null());
            *stack_.back() = v;
        }
    }

    void
    on_document_begin(error_code&) override
    {
        stack_.clear();
        s_.clear();

        root_ = {}; // null
        stack_.push_back(&root_);
    }

    void
    on_object_begin(error_code&) override
    {
        if(stack_.back()->is_object())
        {
            BOOST_ASSERT(! key().empty());
            auto& obj = (
                (*stack_.back())[key().to_string()] =
                    nlohmann::json::object());
            stack_.push_back(&obj);
        }
        else if(stack_.back()->is_array())
        {
            BOOST_ASSERT(key().empty());
            stack_.back()->push_back(
                nlohmann::json::object());
            stack_.push_back(&stack_.back()->back());
        }
        else
        {
            BOOST_ASSERT(stack_.back()->is_null());
            *stack_.back() = nlohmann::json::object();
        }
    }

    void
    on_object_end(error_code&) override
    {
        BOOST_ASSERT(stack_.back()->is_object());
        stack_.pop_back();
    }

    void
    on_array_begin(error_code&) override
    {
        if(stack_.back()->is_object())
        {
            BOOST_ASSERT(! key().empty());
            auto& obj = (
                (*stack_.back())[key().to_string()] =
                    nlohmann::json::array());
            stack_.push_back(&obj);
        }
        else if(stack_.back()->is_array())
        {
            BOOST_ASSERT(key().empty());
            stack_.back()->push_back(
                nlohmann::json::array());
            stack_.push_back(&stack_.back()->back());
        }
        else
        {
            BOOST_ASSERT(stack_.back()->is_null());
            *stack_.back() = nlohmann::json::array();
        }
    }

    void
    on_array_end(error_code&) override
    {
        BOOST_ASSERT(stack_.back()->is_array());
        stack_.pop_back();
    }

    void
    on_string_begin(error_code&) override
    {
        s_.clear();
    }

    void
    on_string_piece(
        string_view s, error_code&) override
    {
        s_.append(s.data(), s.size());
    }

    void
    on_string_end(error_code&) override
    {
        assign(s_);
    }

    void
    on_number(number n, error_code&) override
    {
        if(n.is_integral())
        {
            assign(n.mant);
        }
        else
        {
            long exp = n.pos ?
                n.exp : -static_cast<long>(n.exp);
            auto mul = std::pow(10., exp);
            auto d = (n.neg ? -1. : 1.) * n.mant * mul;
            assign(d);
        }
    }

    void
    on_bool(bool b, error_code&) override
    {
        assign(b);
    }

    void
    on_null(error_code&) override
    {
        assign(nlohmann::json{});
    }
};

class nlohmann_test : public unit_test::suite
{
public:
    using json = nlohmann::json;

    void
    testNlohmann()
    {
        //using namespace nlohmann;
 
        json j;
        j["pi"] = 3.141;
        j["happy"] = true;
        j["name"] = "Niels";
        j["nothing"] = nullptr;
        j["answer"]["everything"] = 42;
        j["list"] = { 1, 0, 2 };
        j["object"] = { {"currency", "USD"}, {"value", 42.99} };
        json j2 =
        {
            {"pi", 3.141},
            {"happy", true},
            {"name", "Niels"},
            {"nothing", nullptr},
            {
                "answer", {
                    {"everything", 42}
                }
            },
            {"list", {1, 0, 2}},
            {
                "object", {
                    {"currency", "USD"},
                    {"value", 42.99}
                }
            }
        };
    }

    void
    changeValue(json& j)
    {
        j = "Hello";
    }

    void
    testUsage()
    {
        //using namespace nlohmann;
        json j;
        changeValue(j[1]);
        //log << j[1] << "\n";
    }

    void
    testParse()
    {
        error_code ec;
        nlohmann_parser p;
        boost::beast::json::parse_file(
            "C:\\Users\\Vinnie\\src\\lounge\\client\\test.json",
            p, ec);
        //log << p.release() << "\n";
    }

    void run() override
    {
        testNlohmann();
        testUsage();
        testParse();
        pass();
    }
};

BEAST_DEFINE_TESTSUITE(beast,nlohmann,nlohmann);

} // beast
} // boost
