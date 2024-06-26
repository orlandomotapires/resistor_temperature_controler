# Resistor Temperature Controller

This project aims to build an automatic controller for the temperature of a resistor using Arduino.

## Authors

- Carlos Alberto Magalhães; [Github](https://github.com/ccasmmjr)
- Eduardo Fiscina; [Github](https://github.com/duducaa)
- Orlando Mota Pires; [Github](https://github.com/orlandomotapires)

## Theoretical Approach

Throughout the continuous evolution of technology, the need to control industrial processes has always been prevalent. The development of increasingly specialized control methods has progressed alongside their growing necessity across various industrial sectors.

Among these methods, temperature regulation stands out as one of the most crucial and well-established. It is responsible for maintaining optimal operational conditions for equipment such as electronic devices, which require precise temperature control to ensure efficiency and longevity. For instance, a deviation of 30 degrees Kelvin from the optimal operating range can significantly impact the efficiency of electronic components. Figure 1 illustrates the relationship between temperature (T) and electrical resistance (R), showing that a 30-degree Kelvin increase causes a 70% decrease in resistive capacity [1].

**Figure 1:** Effect of temperature variation (T) on electrical resistance (R).

<p align="center">
  <img src="./assets/resistence_temperature_relation.png" alt="Resistor Temperature Correlation">
</p>

## Project Description

This project involves creating a circuit with a resistive element powered by an electric voltage and cooled by a small fan. The primary objective is to develop mathematical models that accurately represent its behavior over time and optimize its operational efficiency. The ultimate goal is to implement a control system capable of maintaining the resistor's temperature within an ideal operating range.

### Project Stages

1. Prototype assembly.
2. Analysis of transient behavior of dynamic systems.
3. White-box modeling.
4. Control design via Root Locus method.
5. Control design via empirical methods.
6. Implementation of digital controllers.
7. Signal acquisition via microcontrollers.

## Objective

The main objective is to derive mathematical models for each control system. These models will be developed using various approaches, including Root Locus and other methods.

## General Solution

The project includes MATLAB files in each strategy's folder within the `src` directory, used for plotting graphs and calculating the transfer functions for controlling the resistor's temperature.

## Model 1

### First Approach: Control the fan and analyze its response

- model_1.ino: Evaluates the fan's response to different PWM values, monitoring its RPM to assess how quickly it adjusts to changes in PWM signals.
- model_1.m: This MATLAB script calculates the transfer function and plots the step response for the first control strategy.

## Model 2

### Second Approach: Maintain the fan off and vary the power delivered to the resistor

- model_2.ino: Implements the logic for the second approach in Arduino.
- model_2.m: This MATLAB script calculates the transfer function and plots the step response for the second control strategy.

## Model 3

### Third Approach: Keep the power delivered to the resistor constant while adjusting the fan speed

- model_3.ino: Implements the logic for the third approach in Arduino.
- model_3.m: This MATLAB script calculates the transfer function and plots the step response for the third control strategy.

## References

1. Neto, Antonio & Zolnier, Sérgio. (2006). "Evaluation of an electronic circuit for temperature measurement in agricultural installations using a computer's parallel port." Engenharia Agrícola. 26. [DOI: 10.1590/S0100-69162006000200001](https://doi.org/10.1590/S0100-69162006000200001).