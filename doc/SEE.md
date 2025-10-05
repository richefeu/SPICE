<p align="center">
<img src="./SPICE-logo.png" width="50%"/>
</p>


**SEE** is a visualization tool for exploring particle configurations and simulations. It allows users to load, navigate, and interact with configuration files, visualize particles, forces, and other simulation properties.

**Prerequisites**

- A C++17-compatible compiler
- `freeglut` (and thus `OpenGL`) libraries installed on your system


## Running `see`

`see` should be compiled together with `run` and `pac`.
It allows for visualize any conf-file that can be the input one or the ones generated during a computation.

To visualize a conf-file named `input.txt` for example, you may use the command:

```
see input.txt
```

If the argument is a number `<N>`, `see` will try to open the file `conf<N>`: 

```
see 10
```
If no argument is provided, `see` will try to open `conf0`, which is normally the initial state conf-file of a simulation. 

```
see
```

Once a configuration is visualized, use the keys `+` or `-` to navigate between configuration files.
Most other commands require using the keyboard.
Press `h` to display the help menu for a list of available commands. Press `q` to quit.


## Mouse interaction

Left-Click + Drag,Rotate the view.

Shift + Left-Click + Drag,Pan the view.

Middle-Click + Drag,Zoom in or out.

Right-Click,Open the context menu (e.g., to quit the app).

## Particle colors

...

