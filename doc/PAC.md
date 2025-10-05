<p align="center">
<img src="./SPICE-logo.png" width="50%"/>
</p>

`SPICE` comes with a preprocessing tool referred to as `pac` for setting the Packing And Configuration.

## Usage

Here is a JSON file that describes how the packaging will be built and the properties will be set.

```
{
  "packing-manager": {
    "option": "grid",
    "nx": 20,
    "ny": 40,
    "radius": 0.001,
    "radius-variation": 0.0003,
    "include-far-connection": true,
    "add-bottom-chain": 10,
    "add-top-chain": 10,
    "density": 2700.0
  },
  "properties-manager": {
    "density": 2700.0,
    "normalStiffness": 1e8,
    "tangentialStiffness": 1e8,
    "friction": [[0.25, 0.3],[0.251, 0.8]]
  },
  "pac-options": {
    "generated-file-name": "input.txt",
    "loading": "ShearVV 0 0",
    "verbose": true,
    "gravity": [0.0, -1],
    "time": 0.0,
    "end-time": 1.0,
    "time-step": 1e-6,
    "time-between-update-neighbors": 10e-6,
    "time-between-outputs": 0.01,
    "time-between-conf-files": 0.05,
    "distance-alert": 0.001
  }
}
```

Three parts can be defined as shown in the following example:`packing-manager`, `properties-manager` and `pac-options`.

They are processed in this order (whatever the order in the json file) to produce an input file (a conf-file) that can be used to run an simulation.

```
pac pac.json
```

In this case, the file `input.txt` is created.

## packing-manager




## properties-manager

## pac-options

