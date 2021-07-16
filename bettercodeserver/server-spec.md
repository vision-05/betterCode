# bettercode server specification
## Intro:
The bettercode server communicates over regular TCP sockets with data encoded in edn format. Each message includes an integer (32 bit) size of the message prepending the actual UTF-8 encoded message. This must be used or you will get garbage data. The format of data must take an edn vector (`[]`) with the message/command as the first item and any arguments in order as the rest. As an example one command could be `["insert-text" "/home/foo/bar.txt" 0 "hello"]`



## Commands:
### `"open-file"`
This command opens a file in the buffer, and takes one argument of the file name. Example code to call: `["open-file "/home/foo/bar.txt"]`. Note that paths must be absolute paths (to keep the map of files flat). This command sends back a UTF-8 encoded string of the file contents.

### `"close-file"`
This command closes a file in the buffer (by removing the content without saving). Example code to call: `["close-file" "/home/foo/bar.txt"]`. Again pass absolute paths.

### `"save-file"`
This command saves a file from the buffer, again only taking the filename argument. Example code to call: `["save-file" "/home/foo/bar.txt"]`

### `"insert-text"`
This command will insert a string or characters into the specified filename's content. You must specify the full path, the index and the string or character to insert. Example code to call: `["insert-text" "/home/foo/bar.txt" 2 "foo"]`

### `"remove-text"`
This command will delete a string from a file contents in the buffer, by passing the full path, start and end of the deletion. Example code to call: `["remove-text" "/home/foo/bar.txt" 0 4]`

### `"get-dir"`
This command fetches the files and directories inside the specified path supplied. This will send back a vector of (paths) files followed by the directories, with the string either `"FIL: "` or `"DIR: "` prepended to the path. Example code to call: `["get-dir"]` (no arguments fetches the current working directory), `["get-dir" "/home/foo"]` (gets the contents of `"/home/foo"`). Example return value can be `["FIL: /home/foo/bar.txt" "DIR: /home/foo/baz"]`