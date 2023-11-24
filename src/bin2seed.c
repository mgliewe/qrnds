
#include <stdio.h>
#include <stdint.h>

int main(int argc, char *argv[]) {
	FILE *out = stdout;
	FILE *in;
	unsigned size = 0;
	int col=0;

	fprintf(out, 
		"// automatic generated, do not edit\n"
		"\n"
		"#include <stdint.h>\n"
		"\n"
		"uint64_t builtin_extract_seed[] = {"
	);

	in = fopen(argv[1], "r");
	for (;;) {
		uint64_t v;
		int n = fread(&v, sizeof (uint64_t), 1, in);
		if (!n) break;
		if (col==0) {
			fprintf(out, "\n\t");
		}
		fprintf(out, "0x%lx, ", v);
		if (col++>=4) col=0;
		size++;
	}

	fprintf(out,
		"\n};\n"
		"\n"
		"unsigned builtin_extract_seed_bits = %ld;\n", (size-1) * sizeof(uint64_t)
	);
}

