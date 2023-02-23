// IndexTests.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#pragma warning(disable : 4251)
#include <cstdint>
#include <iostream>
#include <vector>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>
#include <bsoncxx/builder/stream/helpers.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/builder/stream/array.hpp>


using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;

int main()
{
	try
	{
		mongocxx::instance instance{};
		mongocxx::uri uri("mongodb://localhost:27013");
		mongocxx::client client(uri);
		mongocxx::database db = client["HamburgMessdaten"];
		mongocxx::collection coll = db["Test"];

		auto builder = bsoncxx::builder::stream::document{};
		bsoncxx::document::value doc_value = builder
			<< "name" << "MongoDB"
			<< "type" << "database"
			<< "i" << 71
			<< "versions" << bsoncxx::builder::stream::open_array
			<< "v3.2" << "v3.0" << "v2.6"
			<< close_array
			<< "info" << bsoncxx::builder::stream::open_document
			<< "x" << 203
			<< "y" << 102
			<< bsoncxx::builder::stream::close_document
			<< bsoncxx::builder::stream::finalize;

		bsoncxx::stdx::optional<mongocxx::result::insert_one> result =
			coll.insert_one(doc_value.view());

		bsoncxx::stdx::optional<bsoncxx::document::value> maybe_result =
			coll.find_one({});
		if (maybe_result) {
			std::string s = bsoncxx::to_json(*maybe_result);
			std::cout << bsoncxx::to_json(*maybe_result) << "\n";
		}
		/************************** Create Document ******************************/
		//auto doc = bsoncxx::builder::basic::document{};
		//doc.append(
		//	bsoncxx::builder::basic::kvp("name", "TestName"),
		//	bsoncxx::builder::basic::kvp("TheNumber", 13)
		//);
	
		//bsoncxx::stdx::optional<mongocxx::result::insert_one> result =
		//	coll.insert_one(doc.extract());

		/****************************** Explain **********************************/
		//auto command = make_document(kvp("explain",
		//	make_document(kvp("find",
		//		"Test"),
		//		kvp("filter",
		//			make_document(kvp("TheNumber", 13)))
		//	)));

		//auto d = db.run_command(command.view());
		//auto res = bsoncxx::to_json(d);
		//std::cout << res << std::endl;
	
		/***************************** Query ********************************/
		//bsoncxx::stdx::optional<bsoncxx::document::value> maybe_result =
			//coll.find_one(document{} << "TheNumber" << 13 << finalize);
	}
	catch (std::exception e)
	{
		std::cout << e.what();
	}
}