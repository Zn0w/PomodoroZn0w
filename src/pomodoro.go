package main

import (
	//"fmt"
	"net/http"
	"github.com/gorilla/mux"
)

func main() {
	router := mux.NewRouter()
	
	router.HandleFunc("/", handle_index).Methods("GET")

	http.ListenAndServe(":8080", router)
}

func handle_index(w http.ResponseWriter, r *http.Request) {
	w.Header().Set("ContentType", "text/plain; charset=utf-8")
	w.WriteHeader(http.StatusOK)
	w.Write([]byte("Hello! :)"))
}