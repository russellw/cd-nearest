Directory names are only locally unique. In the general case, `cd` requires a full path to unambiguously specify the destination. These things are necessarily true, but for many practical everyday uses, it is also true that the destination could be disambiguated with far fewer characters, and I am nostalgic for the days of Norton CD on MS-DOS, so I finally got around to doing something about it.

Of course on a modern system with tens of thousands of directories, it can't really work by scanning the whole hard disk the way NCD did. Instead, it keeps a list of directories you recently accessed, and chooses the first matching one. So you have to specify the full path the first time:

`c \MyProjects\FirstProject\Subdirectory`

But after that, you can often go back there with a much shorter command like:

`c su`

The command I chose is `c`, for maximum brevity; if you prefer to keep that reserved for something else, feel free to rename `c.bat` accordingly.

The batch file is Windows-specific. Pull requests with shell scripts for other operating systems are welcome. It would also be necessary to conditionally change the location of the recently-accessed list from `%USERPROFILE%\Documents\cd-nearest.csv` to something appropriate for each operating system.

ChatGPT 4o was used in writing this program.
