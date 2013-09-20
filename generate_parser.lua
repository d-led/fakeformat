local platform="windows"

local config    =   {
    compile =  {
        windows="compile.bat"
    }
}

---generate parser---
local res,err=os.execute("chdir smc_format && " .. config.compile[platform])
if res>0 then
    print[[Please fetch smc.jar from http://www.objectmentor.com/resources/downloads.html first!]]
end

local function get_lines(fn,start_placeholder,end_placeholder)
    local ff=assert(io.open(fn,"r"))
    local lines = {}
    local insert_start=-1
    local insert_end=-1
    for line in ff:lines() do
        lines[#lines+1]=line
        if insert_start==-1 and line:find(start_placeholder) then
            insert_start=#lines
        end
        if line:find(end_placeholder) then
            insert_end=#lines
        end
    end
    ff:close()
    print(fn..': read '..#lines..' lines')
    return lines , insert_start , insert_end
end

local function filter_lines(t,start_line,end_line)
    local res={}
    for k,v in ipairs(t) do
        if k>=start_line and k<=end_line then
            res[#res+1]=v
        end
    end
    return res
end

---read fakeformat.hpp---
local ff_lines, ff_insert_start, ff_insert_end = get_lines("fakeformat.hpp",[[///PARSER BEGIN///]],[[///PARSER END///]])

if ff_insert_start<0 or ff_insert_end <0 then
    print[[Cannot find insertion points!]]
    return 1
end

---prepare format_context.h---
local fc_lines, fc_insert_start, fc_insert_end = get_lines("smc_format/format_context.h",[[///BEGIN FORMAT CONTEXT///]],[[///END FORMAT CONTEXT///]])
if fc_insert_start<0 or fc_insert_end <0 then
    print[[Cannot find insertion points!]]
    return 1
end
fc_insert_start=fc_insert_start+1
fc_insert_end=fc_insert_end-1
local format_context_lines=filter_lines(fc_lines,fc_insert_start,fc_insert_end)

---prepare format.h---
local fh_lines, fh_insert_start, fh_insert_end = get_lines("smc_format/format.h",[[#include "format_context.h"]],[[#endif]])
if fh_insert_start<0 or fh_insert_end <0 then
    print[[Cannot find insertion points!]]
    return 1
end
fh_insert_start=fh_insert_start+1
fh_insert_end=fh_insert_end-1
local format_h_lines=filter_lines(fh_lines,fh_insert_start,fh_insert_end)

---prepare format.cpp---
local format_cpp_lines, fp_insert_start, fp_insert_end = get_lines("smc_format/format.cpp",[[//---------]],"\n")

