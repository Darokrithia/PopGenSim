# PopGenSim Linux Install Guide

## Step 1: Install GSL 
```
sudo apt-get install libgsl-dev
```

## Step 2: Run MakeFile
```
make
```

## Step 3: Install pip requirements
```
pip3 install -r requirements.txt
```

## Step 4: Run some example simulations
```
./PolyGenSim
./genancesim
./devosim
```

Try giving it some parameters as well
```
./PolyGenSim -c 10 -p 5 -g 30
```
