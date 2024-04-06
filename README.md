# 1️⃣🐝🏎️ The One Billion Row Challenge

The One Billion Row Challenge (1BRC) is a fun exploration of how far modern Java can be pushed for aggregating one billion rows from a text file.
Grab all your (virtual) threads, reach out to SIMD, optimize your GC, or pull any other trick, and create the fastest implementation for solving this task!

<img src="1brc.png" alt="1BRC" style="display: block; margin-left: auto; margin-right: auto; margin-bottom:1em; width: 50%;">

The text file contains temperature values for a range of weather stations.
Each row is one measurement in the format `<string: station name>;<double: measurement>`, with the measurement value having exactly one fractional digit.
The following shows ten rows as an example:

```
Hamburg;12.0
Bulawayo;8.9
Palembang;38.8
St. John's;15.2
Cracow;12.6
Bridgetown;26.9
Istanbul;6.2
Roseau;34.4
Conakry;31.2
Istanbul;23.0
```

The task is to write a program in multiple languages which reads the file, calculates the min, mean, and max temperature value per weather station, and emits the results on stdout like this
(i.e. sorted alphabetically by station name, and the result values per station in the format `<min>/<mean>/<max>`, rounded to one fractional digit):

```
{Abha=-23.0/18.0/59.2, Abidjan=-16.2/26.0/67.3, Abéché=-10.0/29.4/69.0, Accra=-10.1/26.4/66.4, Addis Ababa=-23.7/16.0/67.0, Adelaide=-27.8/17.3/58.5, ...}
```
## Rules and limits

* Any of these Java distributions may be used:
    * Any builds provided by [SDKMan](https://sdkman.io/jdks)
    * Early access builds available on openjdk.net may be used (including EA builds for OpenJDK projects like Valhalla)
    * Builds on [builds.shipilev.net](https://builds.shipilev.net/openjdk-jdk-lilliput/)
If you want to use a build not available via these channels, reach out to discuss whether it can be considered.
* No external library dependencies may be used
* Implementations must be provided as a single source file
* The computation must happen at application _runtime_, i.e. you cannot process the measurements file at _build time_
(for instance, when using GraalVM) and just bake the result into the binary
* Input value ranges are as follows:
    * Station name: non null UTF-8 string of min length 1 character and max length 100 bytes, containing neither `;` nor `\n` characters. (i.e. this could be 100 one-byte characters, or 50 two-byte characters, etc.)
    * Temperature value: non null double between -99.9 (inclusive) and 99.9 (inclusive), always with one fractional digit
* There is a maximum of 10,000 unique station names
* Line endings in the file are `\n` characters on all platforms
* Implementations must not rely on specifics of a given data set, e.g. any valid station name as per the constraints above and any data distribution (number of measurements per station) must be supported
* The rounding of output values must be done using the semantics of IEEE 754 rounding-direction "roundTowardPositive"

## Entering the Challenge

To submit your own implementation to 1BRC, follow these steps:

* Create a fork of the [onebrc](https://github.com/gunnarmorling/onebrc/) GitHub repository.
* Run `./create_fork.sh <your_GH_user>` to copy the baseline implementation to your personal files, or do this manually:
  * Create a copy of _CalculateAverage\_baseline.java_, named _CalculateAverage\_<your_GH_user>.java_, e.g. _CalculateAverage\_doloreswilson.java_.
  * Create a copy of _calculate\_average\_baseline.sh_, named _calculate\_average\_<your_GH_user>.sh_, e.g. _calculate\_average\_doloreswilson.sh_.
  * Adjust that script so that it references your implementation class name. If needed, provide any JVM arguments via the `JAVA_OPTS` variable in that script.
    Make sure that script does not write anything to standard output other than calculation results.
  * (Optional) OpenJDK 21 is used by default. If a custom JDK build is required, create a copy of _prepare\_baseline.sh_, named _prepare\_<your_GH_user>.sh_, e.g. _prepare\_doloreswilson.sh_. Include the SDKMAN command `sdk use java [version]` in the your prepare script.
  * (Optional) If you'd like to use native binaries (GraalVM), add all the required build logic to your _prepare\_<your_GH_user>.sh_ script.
* Make that implementation fast. Really fast.
* Run the test suite by executing _/test.sh <your_GH_user>_; if any differences are reported, fix them before submitting your implementation.
* Create a pull request against the upstream repository, clearly stating
  * The name of your implementation class.
  * The execution time of the program on your system and specs of the same (CPU, number of cores, RAM). This is for informative purposes only, the official runtime will be determined as described below.
* I will run the program and determine its performance as described in the next section, and enter the result to the scoreboard.

**Note:** I reserve the right to not evaluate specific submissions if I feel doubtful about the implementation (I.e. I won't run your Bitcoin miner ;).

If you'd like to discuss any potential ideas for implementing 1BRC with the community,
you can use the [GitHub Discussions](https://github.com/gunnarmorling/onebrc/discussions) of this repository.
Please keep it friendly and civil.

The challenge runs until Jan 31 2024.
Any submissions (i.e. pull requests) created after Jan 31 2024 23:59 UTC will not be considered.

## Evaluating Results

Results are determined by running the program on a [Hetzner AX161](https://www.hetzner.com/dedicated-rootserver/ax161) dedicated server (32 core AMD EPYC™ 7502P (Zen2), 128 GB RAM).

Programs are run from  a RAM disk (i.o. the IO overhead for loading the file from disk is not relevant), using 8 cores of the machine.
Each contender must pass the 1BRC test suite (_/test.sh_).
The `hyperfine` program is used for measuring execution times of the launch scripts of all entries, i.e. end-to-end times are measured.
Each contender is run five times in a row.
The slowest and the fastest runs are discarded.
The mean value of the remaining three runs is the result for that contender and will be added to the results table above.
The exact same _measurements.txt_ file is used for evaluating all contenders.
See the script _evaluate.sh_ for the exact implementation of the evaluation steps.


## FAQ

_Q: What is the encoding of the measurements.txt file?_\
A: The file is encoded with UTF-8.

_Q: Can I make assumptions on the names of the weather stations showing up in the data set?_\
A: No, while only a fixed set of station names is used by the data set generator, any solution should work with arbitrary UTF-8 station names
(for the sake of simplicity, names are guaranteed to contain no `;` or `\n` characters).

_Q: Can I copy code from other submissions?_\
A: Yes, you can. The primary focus of the challenge is about learning something new, rather than "winning". When you do so, please give credit to the relevant source submissions. Please don't re-submit other entries with no or only trivial improvements.

_Q: Why_ 1️⃣🐝🏎️ _?_\
A: It's the abbreviation of the project name: **One** **B**illion **R**ow **C**hallenge.

## 1BRC on the Web

A list of external resources such as blog posts and videos, discussing 1BRC and specific implementations:

* [The One Billion Row Challenge Shows That Java Can Process a One Billion Rows File in Two Seconds ](https://www.infoq.com/news/2024/01/1brc-fast-java-processing), by Olimpiu Pop (interview)
* [Cliff Click discussing his 1BRC solution on the Coffee Compiler Club](https://www.youtube.com/watch?v=NJNIbgV6j-Y) (video)
* [1️⃣🐝🏎️🦆 (1BRC in SQL with DuckDB)](https://rmoff.net/2024/01/03/1%EF%B8%8F%E2%83%A3%EF%B8%8F-1brc-in-sql-with-duckdb/), by Robin Moffatt (blog post)
* [1 billion rows challenge in PostgreSQL and ClickHouse](https://ftisiot.net/posts/1brows/), by Francesco Tisiot (blog post)
* [The One Billion Row Challenge with Snowflake](https://medium.com/snowflake/the-one-billion-row-challenge-with-snowflake-f612ae76dbd5), by Sean Falconer (blog post)
* [One billion row challenge using base R](https://www.r-bloggers.com/2024/01/one-billion-row-challenge-using-base-r/), by  David Schoch (blog post)
* [1 Billion Row Challenge with Apache Pinot](https://hubertdulay.substack.com/p/1-billion-row-challenge-in-apache), by Hubert Dulay (blog post)
* [One Billion Row Challenge In C](https://www.dannyvankooten.com/blog/2024/1brc/), by Danny Van Kooten (blog post)
* [One Billion Row Challenge in Racket](https://defn.io/2024/01/10/one-billion-row-challenge-in-racket/), by Bogdan Popa (blog post)
* [The One Billion Row Challenge - .NET Edition](https://dev.to/mergeconflict/392-the-one-billion-row-challenge-net-edition), by Frank A. Krueger (podcast)
* [One Billion Row Challenge](https://curiouscoding.nl/posts/1brc/), by Ragnar Groot Koerkamp (blog post)
* [ClickHouse and The One Billion Row Challenge](https://clickhouse.com/blog/clickhouse-one-billion-row-challenge), by Dale McDiarmid (blog post)
* [One Billion Row Challenge & Azure Data Explorer](https://nielsberglund.com/post/2024-01-28-one-billion-row-challenge--azure-data-explorer/), by Niels Berglund (blog post)
* [One Billion Row Challenge - view from sidelines](https://www.chashnikov.dev/post/one-billion-row-challenge-view-from-sidelines), by Leo Chashnikov (blog post)
* [1 billion row challenge in SQL and Oracle Database](https://geraldonit.com/2024/01/31/1-billion-row-challenge-in-sql-and-oracle-database/), by Gerald Venzl (blog post)
* [One Billion Row Challenge: Learned So Far](https://gamlor.info/posts-output/2024-01-12-one-billion-row-challenge/en/), by Roman Stoffel (blog post)
* [One Billion Row Challenge in Racket](https://defn.io/2024/01/10/one-billion-row-challenge-in-racket/), by Bogdan Popa (blog post)
* [The 1 Billion row challenge with Singlestore](https://medium.com/@testily/the-1-billion-row-challenge-with-singlestore-224ce97e451f), by Anna Semjen (blog post)
* [1BRC in .NET among fastest on Linux: My Optimization Journey](https://hotforknowledge.com/2024/01/13/1brc-in-dotnet-among-fastest-on-linux-my-optimization-journey/), by Victor Baybekov (blog post)
* [One Billion Rows – Gerald’s Challenge](https://connor-mcdonald.com/2024/02/03/one-billion-rows-geralds-challenge/), by Connor McDonald (blog post)
* [Reading a file insanely fast in Java](https://rmannibucau.metawerx.net/reading-a-file-insanely-fast-in-java.html), by Romain Manni-Bucau (blog post)
* [#1BRC Timeline](https://tivrfoa.github.io/java/benchmark/performance/2024/02/05/1BRC-Timeline.html), by tivrfoa (blog post)
* [1BRC - What a Journey](https://www.esolutions.tech/1brc-what-a-journey), by Marius Staicu (blog post)
* [One Billion Rows Challenge in Golang](https://www.bytesizego.com/blog/one-billion-row-challenge-go), by Shraddha Agrawal (blog post)
* [The Billion Row Challenge (1BRC) - Step-by-step from 71s to 1.7s](https://questdb.io/blog/billion-row-challenge-step-by-step/) by Marko Topolnik (blog post)
* [Entering The One Billion Row Challenge With GitHub Copilot](https://devblogs.microsoft.com/java/entering-the-one-billion-row-challenge-with-github-copilot/) by Antonio Goncalves (blog post)

## License

This code base is available under the Apache License, version 2.

## Code of Conduct

Be excellent to each other!
More than winning, the purpose of this challenge is to have fun and learn something new.
