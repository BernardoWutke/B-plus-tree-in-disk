{ pkgs }: {
	deps = [
		pkgs.clang_12
		pkgs.ccls
    pkgs.valgrind
		pkgs.gdb
		pkgs.gnumake
	];
}