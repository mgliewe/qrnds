
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


		if line:sub(1,1) ~= '#' then

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
				print("static int show_" .. func_name .. ';')
				print("static " .. return_type .. ptr 
						.. " (* old_" .. func_name .. ") " 
						.. formal_parm .. ';')
			end

			if stage==2 then

				if dont_show[func_name] then
					print("\tshow_" .. func_name .. " = 0;");
				else
					print("\tshow_" .. func_name .. " = 1;");
				end

				-- print("\tp = dlsym(dll, \"" .. func_name .. "\");");
				print("\tp = dlsym(dll, \"" .. func_name .. "\");");
				
				print("\tif(!p) { out(\"FATAL: cannot dlsym of " .. func_name .. "\\n\"); }");
				print("\told_" .. func_name .. " = p;")
				print();
			end

			if stage==3 then

				print(return_type .. " " .. ptr .. func_name .. formal_parm);
				print("{")
				print("\told_" .. func_name .." = dlsym(RTLD_NEXT, \"" .. func_name .."\");")
				print("\tif (last_func!=" .. func_num .. ") repeat_count=0;\n")
				print("\tlast_func = " .. func_num .. "; repeat_count++;")
				print("\tif (repeat_count<=MAX_REPEAT) {")
				print("\t\tif (repeat_count == MAX_REPEAT) {")
				print("\t\t\tfprintf(stderr, \"... repeated call to " .. func_name .. "()\\n\");")
				print("\t\t} else {")
				print("\t\t\tfprintf(stderr, \"" .. return_type .. " " .. func_name .. "()\\n\");")

				for i, name in ipairs(parm_name) do
					local type = trim(parm_type[i])

					if type=='char *' or type=='const char *' then
						print("\t\t\tfprintf(stderr, \"\\t" .. type .. " " .. name .. ": %s\\n\", " .. name ..");")
					elseif type=='HANDLE' or type:sub(-1)=='*' then
						print("\t\t\tfprintf(stderr, \"\\t" .. type .. " " .. name .. ": %p\\n\", " .. name ..");")
					elseif type=='dsp_module_handle' then
						print("\t\t\tfprintf(stderr, \"\\t" .. type .. " "  .. name .. ": %p\\n\", " .. name ..");")
					elseif type=='U8' or type=="ALAZAR_BOARDTYPES" then
						print("\t\t\tfprintf(stderr, \"\\t" .. type .. " "  .. name .. ": %d (0x%02x)\\n\", " 
							.. name .. ", " .. name ..");")
					elseif type=='U16' then
						print("\t\t\tfprintf(stderr, \"\\t" .. type .. " "  .. name .. ": %d (0x%04x)\\n\", " 
							.. name .. ", " .. name ..");")
					elseif type=='U32' or type=="RETURN_CODE" then
						print("\t\t\tfprintf(stderr, \"\\t" .. type .. " "  .. name .. ": %d (0x%08x)\\n\", " 
							.. name .. ", " .. name ..");")
					elseif type=='U64' then
						print("\t\t\tfprintf(stderr, \"\\t" .. type .. " "  .. name .. ": %lld (0x%08llx)\\n\", " 
							.. name .. ", " .. name ..");")
					elseif type=='INT64' or type=="S64" then
						print("\t\t\tfprintf(stderr, \"\\t" .. type .. " "  .. name .. ": %lld (0x%08llx)\\n\", " 
							.. name .. ", " .. name ..");")
					elseif type=='int' or type=='S32' then
						print("\t\t\tfprintf(stderr, \"\\t" .. type .. " "  .. name .. ": %d\\n\", " .. name ..");")
					elseif type=='long' or type=='size_t' then
						print("\t\t\tfprintf(stderr, \"\\t" .. type .. " "  .. name .. ": %ld\\n\", " .. name ..");")
					elseif type=='float' then
						print("\t\t\tfprintf(stderr, \"\\t" .. type .. " "  .. name .. ": %g\\n\", (double)" .. name ..");")
					elseif type=='double' then
						print("\t\t\tfprintf(stderr, \"\\t" .. type .. " "  .. name .. ": %g\\n\", " .. name ..");")
					elseif type=='BOOL' then
						print("\t\t\tfprintf(stderr, \"\\t" .. type .. " "  .. name .. ": %s\\n\", " .. name .."?\"true\":\"false\");")
					else
						print("#error unknown parameter type [" .. type .. ']')
					end
				end

				print("\t\t}")
				print("\t}")
				if return_type=="void" and ptr=="" then
					print("\told_" .. func_name .. "(" .. parm_list .. ");")
				else
					print("\t" .. return_type .. " " .. ptr .. " result = old_" .. func_name .. "(" .. parm_list .. ");")
					print("\tif (repeat_count<MAX_REPEAT) {")

					if return_type=='RETURN_CODE' then
						print("\t\tfprintf(stderr, \"\\t=> %s\\n\", AlazarErrorToText(result));")
					
					elseif return_type..ptr=='U8' then
						print("\t\tfprintf(stderr, \"\\t=> %d (0x%02x)\\n\", result, result);")
					
					elseif return_type..ptr=='U16' then
						print("\t\tfprintf(stderr, \"\\t=> %d (0x%04x)\\n\", result, result);")

					elseif return_type..ptr=='U32' then
						print("\t\tfprintf(stderr, \"\\t=> %d (0x%08x)\\n\", result, result);")

					elseif return_type..ptr=='const char *' then
						print("\t\tfprintf(stderr, \"\\t=> %s\\n\", result);")

					elseif return_type..ptr=='HANDLE' or ptr:sub(-1)=='*' then
						print("\t\tfprintf(stderr, \"\\t=> %p\\n\", result);")
					
					else
						print("#error unknown result type [" .. return_type .. ptr .. ']')
					end

					if func_name=='AlazarGetChannelInfo' then
						print([[
							fprintf(stderr, "\t    memorySize: %d\n", memorySize);
							fprintf(stderr, "\t    bitsPerSample: %d\n", bitsPerSample);
						]]);

					print("\t}")
					print("\treturn result;")
				end
				print("}")
			end
		end
		func_num = func_num + 1
	end
end

stage=1;


print([[
	#define _GNU_SOURCE

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include <stdio.h>

#include "../include/AlazarApi.h"
#include "../include/AlazarDSP.h"

#define API_SO "../lib/libATSApi.so"
//#define API_SO "/lib/x86_64-linux-gnu/libATSApi.so.0"

#define LOGFILE "/tmp/ats-api.log"
#define MAX_REPEAT 4
]])

do_stage()

stage = 2

print([[

static int last_func;
static int repeat_count;

static void out(const char *str) {
	write(2, str, strlen(str));
}


static void load_api_functions() {
	void *p;
	void *dll;

	dll=dlopen(API_SO, RTLD_NOW);
]])

do_stage()
print("}\n")

stage = 3
do_stage()


print([[

//static void init_ats_log() __attribute__((constructor));

void init_ats_log() {
	load_api_functions();
}

]])