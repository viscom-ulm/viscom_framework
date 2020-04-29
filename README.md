# Simple projector color calibration GUI for the VISCOM Framework

In CoordinatorNode.cpp the calibration values are read, changed and saved.
The calibration color for each projector can be set manually with the provided sliders.

The calibration colors can also be generated using a file containing the white point and black point for each projector.
The algorithm works as follows: - Read white points and black points as unsigned 8 bit integers.
                                - Linearize white points and black points.
                                - Substract black points from white points.
                                - Invert and use as multiplicative color masks
The color mask values can in general be larger than 1.0, use the brightness slider to lower the global brightness and adjust the calibration impact.