
const fs = require ("fs");


let def_h = String(fs.readFileSync("qrnd-ui-nodes.h"))
let def_lines = def_h.split("\n")

let list = [];
let cur = null;
let cur_prop = { comment: []}
for (i in def_lines) {
    let line = def_lines[i]
    let re = /.*@ui-([a-zA-Z0-9_]*)[\ ]*(.*)/;
    if (re.test(line)) {
        //console.log(i, re.test(line), line)
        // console.log(re.exec(line))
        let ex=re.exec(line)
        let key = ex[1] 
        let ln = ex[2]
        //console.log(key, ln)


        if (key=='class') {
            // start new component
            if (cur) 
                list.push(cur);
            cur={
                classname: ln,
                input: [],
                output: [],
                defaults: {},
                description: [],
                constructor: []
            };

        } else if (key=="input") {
            cur.input.push(ln)

        } else if (key=="output") {
            cur.output.push(ln)

        } else if (key=="name") {
            // add description paragraph
            cur.shortName = ln

        } else if (key=="description") {
            // add description paragraph
            cur.description.push(ln)

        } else if (key=="property") {
            // add property
            let s = ln.replace(/[\t ]+$/g,'')
                      .replace(/^[\t ]+$/g,'')
                      .replace(/[\t ]+/g,',').split(',')
            let name = s.shift()
            let pr = {
                name: name,
                datatype: s.shift(),
                attributes: {},
                comment: []
            }
            for (i in s) {
                pr.attributes[s[i]] = true;
            }
            if (pr.attributes.ctr)
                cur.constructor.push(name);
            cur.defaults[name] = pr
            cur_prop = pr;

        } else if (key=="comment") {
            // add property comment
            cur_prop.comment.push(ln)

        } else if (key=="default") {
            // add property comment
            cur_prop.value = ln

        } else {
            cur[key] = ln;
        }
    }
}
list.push(cur);

console.log("RED.node_defs = ")
console.log(JSON.stringify(list,0,4))
