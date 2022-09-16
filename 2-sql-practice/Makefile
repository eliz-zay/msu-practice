all: create_directory compile_main compile_test
main: create_directory compile_main
test: create_directory compile_test
create_directory:
	mkdir -p db
	chmod -R 777 db
	mkdir -p out
	mkdir -p out/main
	mkdir -p out/test
compile_main:
	g++ src/client.cpp -w -o out/main/client.out
	g++ src/server.cpp -w -o out/main/server.out
compile_test:
	g++ test/query_parser/CreateQuery.test.cpp -w -o out/test/create.test.out
	g++ test/query_parser/DeleteQuery.test.cpp -w -o out/test/delete.test.out
	g++ test/query_parser/UpdateQuery.test.cpp -w -o out/test/update.test.out
	g++ test/query_parser/SelectQuery.test.cpp -w -o out/test/select.test.out
	g++ test/query_parser/DropQuery.test.cpp -w -o out/test/drop.test.out
	g++ test/query_parser/InsertQuery.test.cpp -w -o out/test/insert.test.out
	g++ test/query_parser/LexicParser.test.cpp -w -o out/test/lexic.test.out
	g++ test/query_processor/conditionTree.test.cpp -w -o out/test/tree.test.out
	g++ test/engine/Engine.test.cpp -w -o out/test/engine.test.out

.PHONY: all test