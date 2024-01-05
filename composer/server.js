/**
 * WARNING: This demo is a barebones implementation designed for development and evaluation
 * purposes only. It is definitely NOT production ready and does not aim to be so. Exposing the
 * demo to the public as is would introduce security risks for the host.
 **/

// @ts-check

const express = require('express');
const expressWs = require('express-ws');
const os = require('os');
const fs = require("fs");
const pty = require('node-pty');
const {
  glob,
  globSync,
  globStream,
  globStreamSync,
  Glob,
} = require('glob');
const { PathScurry, Path } = require('path-scurry')


/** Whether to use binary transport. */
const USE_BINARY = os.platform() !== "win32";

function startServer() {
  const app = express();
  const appWs = expressWs(app).app;

  const terminals = {};
  const unsentOutput = {};
  const temporaryDisposable = {};

  app.use('/xterm.css', express.static(__dirname + '/../css/xterm.css'));
  app.get('/logo.png', (req, res) => {
    res.sendFile(__dirname + '/logo.png');
  });

  app.get('/', (req, res) => {
    res.sendFile(__dirname + '/htdocs/index.html');
  });

  app.use(express.urlencoded());

  function xvfs_router(path) {
    let cwd = path;

    let router = express.Router();

    router.use("/ls", function(req, res) {

      let path = cwd + req.query.path;
      let pattern = req.query.pattern || '*';
  
      let g = globSync('*', {
        cwd: path
      }).map( 
        (fn) => {
          return { 
            name: fn, 
            stat: fs.statSync(path+"/"+fn) 
          };
        } 
      );

      let dirs = g
        .filter( (p) => p.stat.isDirectory() )
        .map( (p) => {
          return { type: 'folder', ...p };
        });

      g = globSync(pattern, {
          cwd: path
        }).map( 
          (fn) => {
            return { 
              name: fn, 
              stat: fs.statSync(path+"/"+fn) 
            };
          } 
        );

      let files = g
      .filter( (p) => p.stat.isFile() )
      .map( (p) => {
        return { type: 'file', ...p };
      });

      res.json({ status: 'success', result: [ ...dirs, ...files ]});
    });

    router.use("/mkdir", function(req, res) {
      let path = cwd + req.query.path;
      try {
        fs.mkdirSync(path, { recursive: true });
        res.json({ status: 'success', result: true });
      } catch {
        res.json({ status: 'error' });
      }
    });

    router.use("/rm", function(req, res) {
      let path = cwd + req.query.path;

      try {
        fs.rmSync(path, { recursive: true });
        res.json({ status: 'success', result: true });
      } catch {
        res.json({ status: 'error' });
      }
    });

    router.use("/readfile", function(req,res) {
      try {
        let path = cwd + req.query.path;
        let content = fs.readFileSync(path, {encoding: 'utf8'});
        res.json({ status: 'success', result: content })
      } catch {
        res.json({ status: 'error' });
      }
    });

    router.use("/writefile", function(req,res) {
      try {
        let path = cwd + req.query.path;
        let content = req.query.content || '';
        console.log("writing "+path);
        fs.writeFileSync(path, content, {encoding: 'utf8'});
        res.json({ status: 'success', result: true })
      } catch(e) {
        console.log(e)
        res.json({ status: 'error' });
      }
    });

    return router;
  }
  app.use("/xvfs" , xvfs_router(__dirname + '/fs/') );

  app.use('/', express.static(__dirname + '/htdocs'));


  let term = null;
  let app_port = null;
  let app_pid = null;

  appWs.ws("/console", function(ws, req) {
    console.log("incoming connection");

    if (app_port) {
      ws.send(JSON.stringify({
        status: "app_startup", pid: app_pid, port: app_port
      }));
    }

    ws.on('close', function() {

    })

    ws.on('message', function(msg) {
      //console.log(msg);

      let cols=80, rows=25;

      let json = JSON.parse(msg);
      switch(json.action) {
        case 'resize':
          cols = json.cols||80;
          rows = json.rows||25;
          if (term) {
            term.resize(cols, rows);
          }
          break;
        case 'start':
          if (term) {
            ws.send(JSON.stringify({ status:'error', msg:'cannot start, already running a process.'}))
          } else {
            ws.send(JSON.stringify({ status:'stderr', msg:'starting ' + json.command}));
            term = pty.spawn(
              "bash", [ "-c", json.command ],
              {
                name: "",
                cols: cols,
                rows: rows,
                cwd: __dirname + '/fs/',
                env: {
                  ...process.env,
                  QRND: __dirname + '/../src',
                  HOME: __dirname + '/fs/',
                  CXX_FLAGS: "-I../../src",
                  QRND_LIBS: "../../src/qrnd.a"
                },
                encoding: 'utf8'
              }
            );
            ws.send(JSON.stringify({ status:"stderr", msg: "started proces "+term.pid}));
            
            term.onData( (data) => {
              ws.send(JSON.stringify( { status:"stdout", msg: data }));

              let re = /\[::pid: ([0-9]*) port: ([0-9]*)::\]/;
              let res = re.exec(data);
              if (res) {
                  app_pid = res[1];
                  app_port = res[2]; 
                  ws.send(JSON.stringify({
                  status: "app_startup", pid: app_pid, port: app_port
                }));
              }
            });

            term.onExit( (ev) => {
              if (ev.signal) {
                ws.send(JSON.stringify({ status:"stderr", msg: "process terminated by signal " + ev.signal }));
              } else {
                ws.send(JSON.stringify({ status:"stderr", msg: "process exited with " + ev.exitCode}));
              }
              term = null;
              app_pid = app_port = null;
            });
          }
          break;
        case 'shutdown':
          if (term) {
            term.kill();
            term=null;
          } else {
            ws.send(JSON.stringify({ status:'error', msg:'cannot shutdown, no process running.'}))
          }
          break;
        default:
          console.log('unknown action: ' + json.action);  
      }
    })

  })

  app.use('/terminal', (req, res) => {
    /** @type {{ [key: string]: string }} */
    const env = {};
    for (const k of Object.keys(process.env)) {
      const v = process.env[k];
      if (v) {
        env[k] = v;
      }
    }
    // const env = Object.assign({}, process.env);
    env['COLORTERM'] = 'truecolor';
    if (typeof req.query.cols !== 'string' || typeof req.query.rows !== 'string') {
      console.error({ req });
      throw new Error('Unexpected query args');
    }
    const cols = parseInt(req.query.cols);
    const rows = parseInt(req.query.rows);
    const term = pty.spawn(process.platform === 'win32' ? 'pwsh.exe' : 'bash', [], {
      name: 'xterm-256color',
      cols: cols ?? 80,
      rows: rows ?? 24,
      cwd: process.platform === 'win32' ? undefined : env.PWD,
      env,
      encoding: USE_BINARY ? null : 'utf8'
    });

    console.log('Created terminal with PID: ' + term.pid);
    terminals[term.pid] = term;
    unsentOutput[term.pid] = '';
    temporaryDisposable[term.pid] = term.onData(function(data) {
      unsentOutput[term.pid] += data;
    });
    res.send(term.pid.toString());
    res.end();
  });

  app.post('/terminal/:pid/size', (req, res) => {
    if (typeof req.query.cols !== 'string' || typeof req.query.rows !== 'string') {
      console.error({ req });
      throw new Error('Unexpected query args');
    }
    const pid = parseInt(req.params.pid);
    const cols = parseInt(req.query.cols);
    const rows = parseInt(req.query.rows);
    const term = terminals[pid];

    term.resize(cols, rows);
    console.log('Resized terminal ' + pid + ' to ' + cols + ' cols and ' + rows + ' rows.');
    res.end();
  });

  appWs.ws('/terminals/:pid', function (ws, req) {
    const term = terminals[parseInt(req.params.pid)];
    console.log('Connected to terminal ' + term.pid);
    temporaryDisposable[term.pid].dispose();
    delete temporaryDisposable[term.pid];
    ws.send(unsentOutput[term.pid]);
    delete unsentOutput[term.pid];

    // unbuffered delivery after user input
    let userInput = false;

    // string message buffering
    function buffer(socket, timeout, maxSize) {
      let s = '';
      let sender = null;
      return (data) => {
        s += data;
        if (s.length > maxSize || userInput) {
          userInput = false;
          socket.send(s);
          s = '';
          if (sender) {
            clearTimeout(sender);
            sender = null;
          }
        } else if (!sender) {
          sender = setTimeout(() => {
            socket.send(s);
            s = '';
            sender = null;
          }, timeout);
        }
      };
    }
    // binary message buffering
    function bufferUtf8(socket, timeout, maxSize) {
      const chunks = [];
      let length = 0;
      let sender = null;
      return (data) => {
        chunks.push(data);
        length += data.length;
        if (length > maxSize || userInput) {
          userInput = false;
          socket.send(Buffer.concat(chunks));
          chunks.length = 0;
          length = 0;
          if (sender) {
            clearTimeout(sender);
            sender = null;
          }
        } else if (!sender) {
          sender = setTimeout(() => {
            socket.send(Buffer.concat(chunks));
            chunks.length = 0;
            length = 0;
            sender = null;
          }, timeout);
        }
      };
    }
    const send = (USE_BINARY ? bufferUtf8 : buffer)(ws, 3, 262144);

    // WARNING: This is a naive implementation that will not throttle the flow of data. This means
    // it could flood the communication channel and make the terminal unresponsive. Learn more about
    // the problem and how to implement flow control at https://xtermjs.org/docs/guides/flowcontrol/
    term.onData(function(data) {
      try {
        send(data);
      } catch (ex) {
        // The WebSocket is not open, ignore
      }
    });
    ws.on('message', function(msg) {
      term.write(msg);
      userInput = true;
    });
    ws.on('close', function () {
      term.kill();
      console.log('Closed terminal ' + term.pid);
      // Clean things up
      delete terminals[term.pid];
    });
  });

  const port = parseInt(process.env.PORT ?? '3000');
  const host = os.platform() === 'win32' ? '127.0.0.1' : '0.0.0.0';

  console.log('App listening to http://127.0.0.1:' + port);
  app.listen(port, host, 0);
}

startServer();
