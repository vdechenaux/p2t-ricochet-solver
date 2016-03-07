# p2t-ricochet-solver

[![Build Status](https://travis-ci.org/vdechenaux/p2t-ricochet-solver.svg?branch=master)](https://travis-ci.org/vdechenaux/p2t-ricochet-solver)

This tool is based on [fogleman/Ricochet](https://github.com/fogleman/Ricochet). Thanks!

## Usage

Simply run:
```
docker run vdechenaux/p2t-ricochet-solver
```

## Options

```
Usage: p2t-ricochet-solver [options]
P2t-ricochet solver

Options:
  -h, --help         Displays this help.
  -v, --version      Displays version information.
  -s, --spoil-me     Shows the best score for each robot.
  -m, --more         Shows the best solution for each robot. Use it with -s or
                     --spoil-me.
  -d, --date <date>  Solves the grid of the given date. The format of <date>
                     must be yyyymmdd. Default value is today date.
```

For example, you can run:
```
docker run vdechenaux/p2t-ricochet-solver --spoil-me --more
```
