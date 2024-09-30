{
  network = {
    description = "web server with ids";
    storage.legacy = {};
  };
  webserver =
    { config, pkgs, ... }:
    {
    deployment.targetEnv = "virtualbox";
    deployment.virtualBox.memorySize = 1024;
    deployment.vcpu = 1;
    defaults = {
      imports = ["./common.nix"];
    };
  };
}
