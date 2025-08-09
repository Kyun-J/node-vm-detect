{
  "targets": [
    {
      "target_name": "vmDetect",
      "sources": [
        "./src/native/main.cpp"
      ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")",
        "./VMAware/src"
      ],
      "dependencies": [
        "<!(node -p \"require('node-addon-api').gyp\")"
      ],
      "defines": [ 'NAPI_CPP_EXCEPTIONS' ],
      "conditions": [
        ["OS=='win'", {
          "libraries": [
            "setupapi.lib",
            "shlwapi.lib",
            "powrprof.lib",
            "tbs.lib"
          ]
        }],
        ["OS=='mac'", {
          "xcode_settings": {
            "OTHER_CFLAGS": ["-std=c++20", "-fexceptions"],
            "OTHER_CPLUSPLUSFLAGS": [ "-std=c++20", "-fexceptions"]
          }
        }],
        ["OS=='linux'", {
          "cflags_cc":  [ "-std=c++20", "-fexceptions" ]
        }]
      ]
    }
  ]
}
