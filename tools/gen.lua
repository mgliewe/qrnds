
local dont_show = {
	AlazarGetOEMFPGAName = true
}

function trim(s)
	return (s:gsub("^%s*(.-)%s*$", "%1"))
 end

function split(pString, pPattern)
	local Table = {}  -- NOTE: use {n = 0} in Lua-5.0
	local fpat = "(.-)" .. pPattern
	local last_end = 1
	local s, e, cap = pString:find(fpat, 1)
	while s do
		if s ~= 1 or cap ~= "" then
			table.insert(Table,trim(cap))
		end
		last_end = e+1
		s, e, cap = pString:find(fpat, last_end)
	end
	if last_end <= #pString then
		cap = pString:sub(last_end)
		table.insert(Table, trim(cap))
	end
	return Table
end

local stage

function do_stage()
	local func_num = 1
	for line in io.lines("func-list.txt") do

		local lst = split(line, "EXPORT")
		local return_type = lst[1]

		lst = split(lst[2], '[(]')
		local func_name = lst[1]
		local ptr = ''
		if string.sub(func_name, 1, 1) == "*" then
			ptr=' *'
			func_name = string.sub(func_name, 2)
		end
		local formal_parm = '(' .. lst[2]
		formal_parm = formal_parm:gsub("^(.-);$", "%1")

		local parm_list = formal_parm:gsub("^%s*[(](.-)[)]%s*$", "%1")
		if parm_list == "void" then parm_list = "" end
		lst = split(parm_list , ',')
		parm_list = ''
		local parm_type = {}
		local parm_name = {}
		for _, str in ipairs(lst) do
			parm_type[_] = str:gsub("^(.-)([a-zA-Z0-9_]*)$", "%1") 
			parm_name[_] = str:gsub("^(.-)([a-zA-Z0-9_]*)$", "%2") 

			if _~=1 then
				parm_list = parm_list .. ", "
			end
			parm_list = parm_list .. parm_name[_]
		end

		

		if stage==1 then
			print("\tint show_" .. func_name .. ';')
			print("\t" .. return_type .. ptr 
			        .. " (*" .. func_name .. ") " 
					.. formal_parm .. ';')
		end

		if stage==2 then

			if dont_show[func_name] then
				print("\tapi_functions.show_" .. func_name .. " = 0;");
			else
				print("\tapi_functions.show_" .. func_name .. " = 1;");
			end

			print("\tp = dlsym(dll, \"" .. func_name .. "\");");
			print("\tif(!p) { out(\"FATAL: cannot dlsym of " .. func_name .. "\"); abort(); }");
			print("\tapi_functions." .. func_name .. " = p;")
			print();
		end

		if stage==3 then

			print(return_type .. " " .. ptr .. func_name .. formal_parm);
			print("{")
			print("\tif (last_func!=" .. func_num .. ") repeat_count=0;\n")
			print("\tlast_func = " .. func_num .. "; repeat_count++;")
			print("\tif (api_functions.show_" .. func_name .. " && repeat_count<=MAX_REPEAT) {")
			print("\t\tFILE *out = fopen(LOGFILE, \"a\");")
			print("\t\tif (repeat_count == MAX_REPEAT) {")
			print("\t\t\tfprintf(out, \"... repeated call to " .. func_name .. "()\\n\");")
			print("\t\t} else {")
			print("\t\t\tfprintf(out, \"" .. return_type .. " " .. func_name .. "()\\n\");")

			for i, name in ipairs(parm_name) do
				local type = trim(parm_type[i])

				if type=='HANDLE' or type:sub(-1)=='*' then
					print("\t\t\tfprintf(out, \"\\t" .. type .. " " .. name .. ": %p\\n\", " .. name ..");")
				elseif type=='dsp_module_handle' then
					print("\t\t\tfprintf(out, \"\\t" .. type .. " "  .. name .. ": %p\\n\", " .. name ..");")
				elseif type=='U8' or type=="ALAZAR_BOARDTYPES" then
					print("\t\t\tfprintf(out, \"\\t" .. type .. " "  .. name .. ": %d (0x%02x)\\n\", " 
						.. name .. ", " .. name ..");")
				elseif type=='U16' then
					print("\t\t\tfprintf(out, \"\\t" .. type .. " "  .. name .. ": %d (0x%04x)\\n\", " 
						.. name .. ", " .. name ..");")
				elseif type=='U32' or type=="RETURN_CODE" then
					print("\t\t\tfprintf(out, \"\\t" .. type .. " "  .. name .. ": %d (0x%08x)\\n\", " 
						.. name .. ", " .. name ..");")
				elseif type=='U64' then
					print("\t\t\tfprintf(out, \"\\t" .. type .. " "  .. name .. ": %lld (0x%08llx)\\n\", " 
						.. name .. ", " .. name ..");")
				elseif type=='INT64' or type=="S64" then
					print("\t\t\tfprintf(out, \"\\t" .. type .. " "  .. name .. ": %lld (0x%08llx)\\n\", " 
						.. name .. ", " .. name ..");")
				elseif type=='int' or type=='S32' then
					print("\t\t\tfprintf(out, \"\\t" .. type .. " "  .. name .. ": %d\\n\", " .. name ..");")
				elseif type=='long' or type=='size_t' then
					print("\t\t\tfprintf(out, \"\\t" .. type .. " "  .. name .. ": %ld\\n\", " .. name ..");")
				elseif type=='float' then
					print("\t\t\tfprintf(out, \"\\t" .. type .. " "  .. name .. ": %g\\n\", (double)" .. name ..");")
				elseif type=='double' then
					print("\t\t\tfprintf(out, \"\\t" .. type .. " "  .. name .. ": %g\\n\", " .. name ..");")
				elseif type=='BOOL' then
					print("\t\t\tfprintf(out, \"\\t" .. type .. " "  .. name .. ": %s\\n\", " .. name .."?\"true\":\"false\");")
				else
					print("#error unknown parameter type [" .. type .. ']')
				end
			end

			print("\t\t}")
			print("\t\tfclose(out);")
			print("\t}")
			if return_type=="void" and ptr=="" then
				print("\tapi_functions." .. func_name .. "(" .. parm_list .. ");")
			else
				print("\t" .. return_type .. " " .. ptr .. " result = api_functions." .. func_name .. "(" .. parm_list .. ");")
				print("\tif (api_functions.show_" .. func_name .. " && repeat_count<MAX_REPEAT) {")
				print("\t\tFILE *out = fopen(LOGFILE, \"a\");")

				if return_type=='RETURN_CODE' then
					print("\t\tfprintf(out, \"\\t=> %s\\n\", api_functions.AlazarErrorToText(result));")
				
				elseif return_type..ptr=='U8' then
					print("\t\tfprintf(out, \"\\t=> %d (0x%02x)\\n\", result, result);")
				
				elseif return_type..ptr=='U16' then
					print("\t\tfprintf(out, \"\\t=> %d (0x%04x)\\n\", result, result);")

				elseif return_type..ptr=='U32' then
					print("\t\tfprintf(out, \"\\t=> %d (0x%08x)\\n\", result, result);")

				elseif return_type..ptr=='const char *' then
					print("\t\tfprintf(out, \"\\t=> %s\\n\", result);")

				elseif return_type..ptr=='HANDLE' or ptr:sub(-1)=='*' then
					print("\t\tfprintf(out, \"\\t=> %p\\n\", result);")
				
				else
					print("#error unknown result type [" .. return_type .. ptr .. ']')
				end

				print("\t\tfclose(out);")
				print("\t}")
				print("\treturn result;")
			end
			print("}")
		end

		func_num = func_num + 1
	end
end

stage=1;


print([[
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include <stdio.h>

#include "../include/AlazarApi.h"
#include "../include/AlazarDSP.h"

//#define API_SO "../lib/libATSApi.so"
#define API_SO "/lib/x86_64-linux-gnu/libATSApi.so.0"

#define LOGFILE "/tmp/ats-api.log"
#define MAX_REPEAT 4

struct api_functions {
]])
do_stage()
print('};\n')

stage = 2

print([[

static struct api_functions api_functions;
static void *dll;

static int last_func;
static int repeat_count;

static void out(const char *str) {
	write(2, str, strlen(str));
}


static void load_api_functions() {
	void *p;
	dll = dlopen(API_SO, RTLD_NOW);
	if (!dll) {
		out("FATAL: cannot dlopen "); out(API_SO); out("\n");
		abort();
	}
]])
do_stage()
print("}\n")

stage = 3
do_stage()


print([[

static void init_ats_log() __attribute__((constructor));

void init_ats_log() {
	load_api_functions();
}

]])