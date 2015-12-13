# Tiny Arcade Simulator

This is an embryo of a simulator for Tiny Arcade, to simplify development of new and cool games.

## Goal

The goal is to be able to compile the same code for both the real Tiny Arcade hardware and
the simulator. 

## Implementation

To allow users on different platforms I chose to implemented it using SDL2. 


### Current status

 * Simple SPI and I2C simulation. Only one I2C slave supported at the moment.
 * TinyDisplay basic simulation.


### Planned features
 * TinyDisplay hardware accellerated commands simulated
 * Simulate Serial interface for printing logs.
 * Simulate support for basic arduino functionality like Delay function.
 * Implement sound output.
 * Joystick and button input simulation.

