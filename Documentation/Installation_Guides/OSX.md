# PopGenSim OSX Install Guide

## Step 0: Install Homebrew

This guide assumes you already have Homebrew enabled and set up.
If you do not already have Homebrew installed on your Mac, please check out the
[Homebrew Installation Documentation](https://docs.brew.sh/Installation)

## Step 1: Install GSL 
```
brew install gsl
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
