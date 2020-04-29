# Simple projector color calibration GUI for the VISCOM Framework

In CoordinatorNode.cpp the calibration values are read, changed and saved.
The calibration color for each projector can be set manually with the provided sliders.

The calibration colors can also be generated using a file containing the white point and black point for each projector (colors_from_image_example.txt shows the format of such a file).
The algorithm returns a rgb mask color for each projector and works as follows:

- Read white points and black points as unsigned 8 bit integers.
- Linearize white points and black points.
- Substract black points from white points.
- Invert and normalize with the minimal color value.

The color mask values are normalized and can be used multiplicatively. Use the brightness slider to adjust the global brightness and the calibration impact.