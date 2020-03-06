package main

import (
	"fmt"
	"os"
	"bufio"
	"runtime"
	"os/exec"
	"os/signal"
	//"time"
)

type Mode int

const (
	Console Mode = 0
	TimerRunning = 1
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
	// Set up channel on which to send signal notifications.
	// We must use a buffered channel or risk missing the signal
	// if we're not ready to receive when the signal is sent.
	signal_channel := make(chan os.Signal, 1)
	signal.Notify(signal_channel, os.Interrupt)
	s := <-signal_channel
	fmt.Println("Got signal:", s)
	
	running := true
	scanner := bufio.NewScanner(os.Stdin)
	mode := Console

	for running {
		switch mode {
		case Console:
			if scanner.Scan() {
				if scanner.Text() == "quit" || scanner.Text() == "exit" {
					running = false
				} else if scanner.Text() == "new" {
					mode = TimerRunning
				} else {
					fmt.Println("Unresolved command")
				}
			}
		case TimerRunning:
			Clear()
			fmt.Println("time display")
			if scanner.Scan() {
				if scanner.Text() == "stop" {
					mode = Console
				}
			}
		}
	}

	fmt.Println("Exiting Pomodoro")
}