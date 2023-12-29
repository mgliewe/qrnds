
/** https://previews.envatousercontent.com/files/225058891/index.html
 * 
 */
!function(a, b, c, d) {
    "use strict";
    var widget_name = "simpleFileBrowser";
    function simpleFileBrowser(el, settings) {
        this.element = el,
        this.$element = jQuery(el),
        this.name = widget_name,
        this.opts = jQuery.extend(!0, {}, jQuery.fn[widget_name].defaults, settings),
        this.$elements = {
            breadcrumbs: null,
            content: null
        };
        this.init(b, settings)
    }
    simpleFileBrowser.prototype = {
        _get: function() {
            //console.log(this)
            var _this = this;
            if (_this.opts.json) {
                var c = jQuery.Deferred();
                return c.resolve(_this.opts.json[_this._pathToString()]),
                       c.promise()
            } else if (_this.opts.ls_func) {
                return _this.opts.ls_func(_this._pathToString(), _this.opts.pattern);
            } else {
                var d = _this.opts.data;
                d.pattern = _this.opts.pattern || '*';
                return d.path = _this._pathToString(),
                jQuery.ajax({
                    url: _this.opts.url,
                    dataType: "json",
                    type: _this.opts.method,
                    async: !0,
                    cache: !1,
                    data: d
                })    
            }
        },
        _awesome: function(a) {
            var cls = "";
            switch (a) {
            case "folder":
                cls = "fa-thin fa-folder";
                break;
            case "ogg":
            case "wav":
            case "mp3":
                cls = "fa-thin fa-file-audio";
                break;
            case "docx":
            case "doc":
            case "odt":
                cls = "fa-thin fa-file-word";
                break;
            case "xls":
                cls = "fa-thin fa-file-excel";
                break;
            case "pps":
                cls = "fa-thin fa-file-powerpoint";
                break;
            case "tiff":
            case "gif":
            case "bmp":
            case "png":
            case "jpeg":
            case "jpg":
                cls = "fa-thin fa-file-image";
                break;
            case "gz":
            case "tar":
            case "rar":
            case "zip":
                cls = "fa-thin fa-file-archive";
                break;
            case "mpg":
            case "avi":
            case "mp4":
            case "flv":
                cls = "fa-thin fa-archive";
                break;
            case "pdf":
                cls = "fa-thin fa-pdf";
                break;
            case "txt":
            case "sql":
            case "php":
            case "css":
            case "js":
            case "config":
                cls = "fa-thin fa-file-text";
                break;
            default:
                cls = "fa-thin fa-file"
            }
            return cls
        },
        _trim: function(a, b, c) {
            return b === undefined && (b = "s"),
            c === undefined && (c = "both"),
            ("left" == c || "both" == c) && (a = a.replace(new RegExp("^[" + b + "]+"), "")),
            ("right" == c || "both" == c) && (a = a.replace(new RegExp("[" + b + "]+$"), "")),
            a
        },
        _pathChange: function(a) {
            var _this = this;
            a != undefined && (".." == a ? _this.path.pop() : _this.path.push(a))
        },
        _pathToString: function(a) {
            var _this = this;
            if (a === undefined)
                var c = _this.path;
            else
                var c = _this.path.slice(0, a);
            var e = 1 == c.length ? "/" : c.join("/");
            return e
        },
        _draw: function(b) {
            var c = this;
            c.$elements.content.html(""),
            c.$element.removeClass("x32 x22 x16"),
            c.$element.addClass("x" + c.opts.size),
            c.$elements.content.removeClass("icon details"),
            c.$elements.content.addClass(c.opts.view),
            c.opts.breadcrumbs && (c.$elements.breadcrumbs.html(""),
            jQuery.each(c.path, function(b, d) {
                var e = ("" === d) ? "root" : d
                  , f = jQuery("<li />").data("path", c._pathToString(b + 1)).text(e);
                c.$elements.breadcrumbs.append(f)
            }));
            var d = "<ul>";
            jQuery.each(b, function(a, b) {
                d += '<li data-name="' + b.name + 
                     '" data-type="' + ("folder" == b.type ? "folder" : "file") + 
                     '"><i class="' + c._awesome(b.type) + '"></i><span>' + (b.name || "") + 
                     "</span></li>"
            }),
            d += "</ul>",
            c.$elements.content.html(d)
        },
        init: function() {
            var _this = this;
            _this.path = _this._trim(this.opts.path, "/", "right").split("/"),
            _this.$element.html(""),
            _this.$element.addClass("sfb"),
            1 == _this.opts.breadcrumbs && (_this.$elements.breadcrumbs = jQuery("<ul />").addClass("sfbBreadCrumbs"),
            _this.$element.append(_this.$elements.breadcrumbs),
            _this.$elements.breadcrumbs.on("click", "li", function() {
                var c = jQuery(this);
                _this.path = _this._trim(c.data("path"), "/", "right").split("/"),
                _this._get().then(function(a) {
                    _this._draw(a)
                    _this.opts.onOpen.call(_this, _this, '', _this._pathToString(), 'folder')
                })
            })),
            _this.$elements.content = jQuery("<div />").addClass("sfbContent"),
            _this.$element.append(_this.$elements.content);
            var c = function(a) {
                "folder" == a.data("type") && (_this._pathChange(a.data("name")),
                _this._get().then(function(a) {
                    _this._draw(a)
                })),
                _this.opts.onOpen.call(_this, _this, a.data("name"), _this._pathToString(), a.data("type"))
            }
              , d = function(a) {
                ".." != a.data("name") && (_this.$elements.content.find("li").removeClass("selected"),
                a.addClass("selected"),
                _this.selected = _this._pathToString() + a.data("name"),
                _this.opts.onSelect.call(_this, _this, a.data("name"), _this._pathToString(), a.data("type")))
            }
              , e = 0
              , f = 1;
            _this.opts.select ? 2 : 1;
            _this.$elements.content.on("click", "li", function() {
                var g = jQuery(this);
                e++,
                1 === e ? f = setTimeout(function() {
                    _this.opts.select ? d(g) : c(g),
                    e = 0
                }, 200) : (clearTimeout(f),
                _this.opts.select && d(g),
                c(g),
                e = 0)
            }),
            _this._get().then(function(a) {
                _this._draw(a),
                _this.opts.onLoad.call(_this, _this)
            })
        },
        remove: function() {
            this.html(""),
            this.$element.removeData("plugin_" + widget_name),
            this.$element.removeClass("sfb x32 x22 x16")
        },
        getSelected: function() {
            var a = this;
            return a.selected
        },
        getPath: function() {
            var a = this;
            return a._pathToString()
        },
        setPath: function(p) {
            var _this = this;
            console.log(p, _this)
            return _this._pathToString()
        },
        redraw: function() {
            var a = this;
            a._get().then(function(b) {
                a._draw(b)
            })
        },
        chgOption: function(a) {
            var _this = this;
            "undefined" != typeof a.view && (_this.opts.view = a.view),
            "undefined" != typeof a.size && (_this.opts.size = a.size),
            "undefined" != typeof a.select && (_this.opts.select = a.select),
            "undefined" != typeof a.path && (_this.path = _this._trim(a.path, "/", "right").split("/")),
            _this.redraw()
        }
    },
    jQuery.fn[widget_name] = function(arg) {
        if ("string" == typeof arguments[0]) {
            var method = arg;
            var c = Array.prototype.slice.call(arguments, 1);
            if (jQuery.isFunction(simpleFileBrowser.prototype[method])) {
                var d;
                return this.each(function() {
                    d = jQuery.data(this, "plugin_" + widget_name)[method](c[0])
                }),
                "undefined" != typeof d ? d : this
            }
            jQuery.error("Method " + method + " is not available")
        } else if ("object" == typeof method || !method)
            var settings = arg;
            return this.each(function() {
                jQuery.data(this, "plugin_" + widget_name)  
                            || jQuery.data(this, "plugin_" + widget_name, new simpleFileBrowser(this,settings))
            })
    }
    ,
    jQuery.fn[widget_name].defaults = {
        url: "folder.php",
        json: null,
        method: "post",
        view: "details",
        size: "32",
        path: "/",
        breadcrumbs: !1,
        select: !0,
        data: {},
        onSelect: function() {},
        onOpen: function() {},
        onDestroy: function() {},
        onLoad: function() {}
    }
}(jQuery, window, document);
