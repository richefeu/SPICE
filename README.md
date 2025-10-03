<p align="center">
<img src="./doc/SPICE-logo.png" width="50%"/>
</p>

## What is `SPICE`

`SPICE`, for Semi-Periodic Interface Cell Element, is a 2D-DEM code that handles periodic boundary conditions along the horizontal axis and simulates a far-away field effect vertically. Originally developed for studying granular interfaces, particularly those characterized by gradients in granular material properties, SPICE offers a unique approach to modeling such complex particle systems.


`SPICE` is originally developed by Vincent Richefeu and Cyrille Couture at the Laboratory 3SR. Contributions from other developers are welcome in future updates.

## To be done

- add properties (rupture energy, friction coeff, stiffness...) in interactions or bodies

- develop the application that assemble the packing in different manner (gradient of size, gradient of failure energy...)

- maybe add viscosity (or more) to the far-field

- check wether some numerical dissipation is required (maybe not)

- maintain documentation




- direction horizontale permettre d'appliquer une contrainte (autrement dit permettre une largueur periodique variable) -> K0 ?...

- mettre au claire le calcul de la contrainte moyenne sur la cellule complète (que faire avec les forces entre les croix et les particules liées au farfield)

- d'ailleurs le terme de farfield n'est peut-êtyre pas le bon


