# PopGenSim Windows Install Guide

## Step 0: Install WSL

This guide assumes you already have Windows Subsystems Fox Linux enabled and set up.
If you do not already have a linux shell running on your windows computer, please check out 
this handy WSL installation guide from Michael Treat.
[WSL Installation Guide](https://github.com/michaeltreat/Windows-Subsystem-For-Linux-Setup-Guide)

## Step 1: Install GSL 
```
sudo apt-get install libgsl-dev
```

## Step 2: Run MakeFile
```
make
```

## Step 3: Run some example simulations
```
./PolyGenSim
./genancesim
./devosim
```

Try giving it some parameters as well
```
./PolyGenSim -c 10 -p 5 -g 30
```
