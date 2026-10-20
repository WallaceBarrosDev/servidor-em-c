cmake.build_dir := build
cmake.banner := bin/program

.PHONY: app clean

app:
	@cmake -S . -B $(cmake.build_dir) -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
	@cmake --build $(cmake.build_dir)
	@./bin/program

clean:
	@rm -rf $(cmake.build_dir) bin
