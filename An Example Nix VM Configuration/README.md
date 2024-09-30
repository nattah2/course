# Introduction

It makes a VM with an IDS (Snort).

```nix
nixops create ./nixops.nix -d webserver-with-ids
nixops deploy -d webserver-with-ids
nixops ssh webserver -d webserver-with-ids
```

