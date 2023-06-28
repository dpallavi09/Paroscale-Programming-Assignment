package main

import (
"fmt"
"os"
"path/filepath"
"sync"
)

type DirStats struct {
Files        int
Subdirectories int
}

func countFilesAndSubdirectories(path string, wg *sync.WaitGroup, ch chan DirStats) {
defer wg.Done()

files := 0
subdirectories := 0

err := filepath.Walk(path, func(p string, info os.FileInfo, err error) error {
if err != nil {
return err
}

if info.IsDir() {
subdirectories++
} else {
files++
}

return nil
})

if err != nil {
fmt.Printf("Error scanning directory %s: %s\n", path, err.Error())
return
}

ch <- DirStats{Files: files, Subdirectories: subdirectories}
}

func main() {
rootDir := "/path/to/directory"

var wg sync.WaitGroup
ch := make(chan DirStats)

wg.Add(1)
go countFilesAndSubdirectories(rootDir, &wg, ch)

go func() {
wg.Wait()
close(ch)
}()

totalFiles := 0
totalSubdirectories := 0

for dirStats := range ch {
fmt.Printf("Directory: %d files, %d subdirectories\n", dirStats.Files, dirStats.Subdirectories)
totalFiles += dirStats.Files
totalSubdirectories += dirStats.Subdirectories
}

fmt.Printf("Total: %d files, %d subdirectories\n", totalFiles, totalSubdirectories)
}
