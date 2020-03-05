package main

import (
	"fmt"
	"os"
	//"bufio"
	"runtime"
	"os/exec"
	"time"
)

func Clear() {
	if runtime.GOOS == "linux" {
		cmd := exec.Command("clear")
		cmd.Stdout = os.Stdout
		cmd.Run()
	} else if runtime.GOOS == "windows" {
		cmd := exec.Command("cmd", "/c", "cls")
		cmd.Stdout = os.Stdout
		cmd.Run()
	}
}

func main() {
	for {
		fmt.Println("A")
		time.Sleep(1 * time.Second)
		Clear()
		fmt.Println("B")
		time.Sleep(1 * time.Second)
		Clear()
	}
}