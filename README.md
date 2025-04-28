# qulacs-bridge

"medium" level bindings to qulacs cppsim using cxxbridge.

## Development

The recommended way to develop is using [devenv.sh](https://devenv.sh/) and [direnv](https://direnv.net/).

With devenv and direnv installed you can simply allow the enviroment and then run

```
  cargo build
```

Or similar cargo commands without issue.

If you do not wish to use devenv you can instead set the QULACS_PATH environment variable to the
location of a qulacs install that contains a built copy of the library.
