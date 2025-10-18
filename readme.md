# probabilityx:awcustom
This branch builds a calculator designed to evaluate Advance Wars By Web unit matchups.

After building, the program can be run by supplying the instructions to the standard input of the program. 

```
./AWCalculator < template.aw
```

It can be run interactively at the command line, although syntax errors will cause the program to terminate immediately.

## Requirements
Builds with C++ Boost 1.89 and GCC 14.

Build directory can be setup with the following commands:

```
mkdir build && cd $_
cmake .. -DCMAKE_BUILD_TYPE=Release -DBoost_ROOT=<path_to_boost_1.89>
```

The application can then be run from the build directory, or copied to a different directory to be run in.

The exact Boost version can be a lower version, but the cmake config will need to be changed to match the version being compiled with, or else the compilation will fail. Some older versions of the Boost.Multiprecision library are known to have defects that specifically affect the functionality used in this library, so using at least 1.89 is preferred.

## Example
An example script:

```
# Context: Sonja is trying to capture a neutral base, and Adder doesn't want to let her. He has two infantry in position, and tries to interrupt
SonjaCaptInf:infantry|base|sonja|||||||
AdderIntInf1:infantry|shoals|adder|||||||

# This move already happened. In-game, we observed that the attacker and defender both ended up at 6 HP Units
# So now we filter the results to determine what the actual distribution of hp is
# We use two filters: 'defenderhp<51,60<between' and 'attackerhp<51,60<between'.
# Since we already know what the visible outcome was, we can discard any outcomes that don't match.
AdderIntInf1>SonjaCaptInf>defenderhp<51,60<between>attackerhp<51,60<between

echo!### In-Game Results ###
print!AdderIntInf1
print!SonjaCaptInf

# Now, we have a second infantry show up to try to finish the job.
AdderIntInf2:infantry|shoals|adder|||||||

# In-game, the attacking infantry didn't (appear) to take any damage, and the defending infantry was reduced to 1HP
AdderIntInf2>SonjaCaptInf>attackerhp<91<atleast>defenderhp<10<atmost

print!AdderIntInf2
print!SonjaCaptInf

# Now, we roll it all back, and just find out all the possible ranges of outcomes.
SonjaCaptInf:infantry|base|sonja|||||||
AdderIntInf1:infantry|shoals|adder|||||||
AdderIntInf2:infantry|shoals|adder|||||||
AdderIntInf1>SonjaCaptInf
AdderIntInf2>SonjaCaptInf

echo!
echo!
echo!### Theoretical Results ###
print!SonjaCaptInf
print!AdderIntInf1
print!AdderIntInf2
```

The output it produces:

```
### In-Game Results ###
AdderIntInf1: infantry on shoals controlled by adder, in state 0 with ammo, with 0 towers and 0 properties, at (approximately) 53 Hitpoints.
    HP :    Odds      Dead%    Alive%
  [ 51]:  18.182%   18.182%  100.000%
  [ 52]:  30.909%   49.091%   81.818%
  [ 53]:  23.636%   72.727%   50.909%
  [ 54]:   9.091%   81.818%   27.273%
  [ 55]:  12.727%   94.545%   18.182%
  [ 56]:   5.455%  100.000%    5.455%
SonjaCaptInf: infantry on base controlled by sonja, in state 0 with ammo, with 0 towers and 0 properties, at (approximately) 58 Hitpoints.
    HP :    Odds      Dead%    Alive%
  [ 56]:  28.571%   28.571%  100.000%
  [ 57]:  14.286%   42.857%   71.429%
  [ 58]:  28.571%   71.429%   57.143%
  [ 59]:  14.286%   85.714%   28.571%
  [ 60]:  14.286%  100.000%   14.286%
AdderIntInf2: infantry on shoals controlled by adder, in state 0 with ammo, with 0 towers and 0 properties, at (approximately) 92 Hitpoints.
    HP :    Odds      Dead%    Alive%
  [ 91]:   6.000%    6.000%  100.000%
  [ 92]:  73.000%   79.000%   94.000%
  [ 93]:  21.000%  100.000%   21.000%
SonjaCaptInf: infantry on base controlled by sonja, in state 0 with ammo, with 0 towers and 0 properties, at (approximately) 8 Hitpoints.
    HP :    Odds      Dead%    Alive%
  [  4]:   4.545%    4.545%  100.000%
  [  5]:   6.818%   11.364%   95.455%
  [  6]:  15.909%   27.273%   88.636%
  [  7]:  15.909%   43.182%   72.727%
  [  8]:  20.455%   63.636%   56.818%
  [  9]:  18.182%   81.818%   36.364%
  [ 10]:  18.182%  100.000%   18.182%


### Theoretical Results ###
SonjaCaptInf: infantry on base controlled by sonja, in state 0 with ammo, with 0 towers and 0 properties, at (approximately) 11 Hitpoints.
    HP :    Odds      Dead%    Alive%
  [  4]:   2.000%    2.000%  100.000%
  [  5]:   3.000%    5.000%   98.000%
  [  6]:   7.000%   12.000%   95.000%
  [  7]:   7.000%   19.000%   88.000%
  [  8]:   9.000%   28.000%   81.000%
  [  9]:   8.000%   36.000%   72.000%
  [ 10]:   8.000%   44.000%   64.000%
  [ 11]:  11.000%   55.000%   56.000%
  [ 12]:  11.000%   66.000%   45.000%
  [ 13]:  10.000%   76.000%   34.000%
  [ 14]:   6.000%   82.000%   24.000%
  [ 15]:   5.000%   87.000%   18.000%
  [ 16]:   5.000%   92.000%   13.000%
  [ 17]:   4.000%   96.000%    8.000%
  [ 18]:   3.000%   99.000%    4.000%
  [ 19]:   1.000%  100.000%    1.000%
AdderIntInf1: infantry on shoals controlled by adder, in state 0 with ammo, with 0 towers and 0 properties, at (approximately) 49 Hitpoints.
    HP :    Odds      Dead%    Alive%
  [ 36]:   0.300%    0.300%  100.000%
  [ 37]:   0.600%    0.900%   99.700%
  [ 38]:   0.900%    1.800%   99.100%
  [ 39]:   2.700%    4.500%   98.200%
  [ 40]:   1.800%    6.300%   95.500%
  [ 41]:   4.500%   10.800%   93.700%
  [ 42]:   2.700%   13.500%   89.200%
  [ 43]:   5.700%   19.200%   86.500%
  [ 44]:   2.400%   21.600%   80.800%
  [ 45]:   2.100%   23.700%   78.400%
  [ 46]:   5.400%   29.100%   76.300%
  [ 47]:   6.100%   35.200%   70.900%
  [ 48]:   5.000%   40.200%   64.800%
  [ 49]:   9.400%   49.600%   59.800%
  [ 50]:  11.900%   61.500%   50.400%
  [ 51]:   7.000%   68.500%   38.500%
  [ 52]:  11.900%   80.400%   31.500%
  [ 53]:   9.100%   89.500%   19.600%
  [ 54]:   3.500%   93.000%   10.500%
  [ 55]:   4.900%   97.900%    7.000%
  [ 56]:   2.100%  100.000%    2.100%
AdderIntInf2: infantry on shoals controlled by adder, in state 0 with ammo, with 0 towers and 0 properties, at (approximately) 88 Hitpoints.
    HP :    Odds      Dead%    Alive%
  [ 82]:   1.680%    1.680%  100.000%
  [ 83]:  14.000%   15.680%   98.320%
  [ 84]:  24.640%   40.320%   84.320%
  [ 85]:  14.000%   54.320%   59.680%
  [ 86]:   1.680%   56.000%   45.680%
  [ 91]:   2.640%   58.640%   44.000%
  [ 92]:  32.120%   90.760%   41.360%
  [ 93]:   9.240%  100.000%    9.240%

```
