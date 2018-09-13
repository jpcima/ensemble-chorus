# JPC Ensemble Chorus
Digital model of electronic string ensemble chorus

![screenshot](docs/screen.png "GUI overview")

**Experimental!**

## Sound demo

Listen to the biniou:

- [Original Sound](https://soundcloud.com/user-482248552/jpc-ensemble-chorus-demo)
- [Chorused Sound](https://soundcloud.com/user-482248552/ensemble-chorus-demo)

## Development builds

[![Build Status](https://semaphoreci.com/api/v1/jpcima/ensemble-chorus/branches/master/badge.svg)](https://semaphoreci.com/jpcima/ensemble-chorus)

Find automatic builds of the development branch [here](http://jpcima.sdf1.org/software/development/EnsembleChorus/).

## Dependencies

- `git`
- `build-essential`
- `cmake`
- `fluid`
- `libfltk1.3-dev`
- `libasound2-dev`
- `libjack-jackd2-dev` or `libjack-dev` (for Jack 2 or Jack 1 respectively)
- `libpulse-dev` (optional)

## Build

```
git clone --recursive https://github.com/jpcima/ensemble-chorus.git
mkdir ensemble-chorus/build
cd ensemble-chorus/build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
```

## References

Raffel, C., & Smith, J. (2010, September)  
[Practical modeling of bucket-brigade device circuits](http://colinraffel.com/publications/dafx2010practical.pdf)

Jurgen Haible (2007)  
[JH. String Ensemble / Triple Chorus - "Solina"(TM) Emulator](http://jhaible.com/legacy/triple_chorus/triple_chorus.html)
