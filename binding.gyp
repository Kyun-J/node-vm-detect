{
  "conditions": [
    ["OS=='win'", {
      "targets": [
        {
          "target_name": "detectWindows",
          "sources": [
            "./VMAware/src/vmaware.hpp",
            "./src/vmDetect.cpp",
            "./src/main.cpp"
          ],
          "libraries": [
            "setupapi.lib",
            "shlwapi.lib",
            "powrprof.lib",
            "tbs.lib"
          ],
          "include_dirs": [
            "<!@(node -p \"require('node-addon-api').include\")",
            "./VMAware/src"
          ],
          "dependencies": [
            "<!(node -p \"require('node-addon-api').gyp\")"
          ],
          "defines": [ 'NAPI_CPP_EXCEPTIONS' ]
        }
      ]
    }],
    ["OS=='mac'", {
      "targets": [
        {
          "target_name": "detectMac",
          "sources": [
            "./VMAware/src/vmaware.hpp",
            "./src/vmDetect.cpp",
            "./src/main.cpp"
          ],
          "include_dirs": [
            "<!@(node -p \"require('node-addon-api').include\")",
            "./VMAware/src"
          ],
          "dependencies": [
            "<!(node -p \"require('node-addon-api').gyp\")"
          ],
          "defines": [ 'NAPI_CPP_EXCEPTIONS' ],
          "xcode_settings": {
            "OTHER_CFLAGS": ["-std=c++20", "-fexceptions"],
            "OTHER_CPLUSPLUSFLAGS": [ "-std=c++20", "-fexceptions"]
          }
        }
      ]
    }],
    ["OS=='linux'", {
      "targets": [
        {
          "target_name": "detectLinux",
          "sources": [
            "./VMAware/src/vmaware.hpp",
            "./src/vmDetect.cpp",
            "./src/main.cpp"
          ],
          "include_dirs": [
            "<!@(node -p \"require('node-addon-api').include\")",
            "./VMAware/src"
          ],
          "dependencies": [
            "<!(node -p \"require('node-addon-api').gyp\")"
          ],
          "defines": [ 'NAPI_CPP_EXCEPTIONS' ],
          "cflags_cc":  [ "-fexceptions", "-std=c++20" ]
        }
      ]
    }]
  ]
}
