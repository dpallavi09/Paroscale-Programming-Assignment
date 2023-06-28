set file [open "top_output.txt" r]
set top_output [read $file]
close $file
set pattern {^\s*(\d+)\s+\w+.*?\s+(\w+)\s*$}
set matches [regexp -all -inline -lineanchor $pattern $top_output]
foreach {line pid user} $matches {
    puts "PID: $pid | User: $user"
}
