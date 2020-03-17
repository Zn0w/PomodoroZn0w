package main

import (
	//"fmt"
	"net/http"
    "github.com/gorilla/mux"
    "html/template"
    "time"
)

type Context struct {
    Date string
}

func main() {
	router := mux.NewRouter()
	
    router.HandleFunc("/", handle_index).Methods("GET")
    router.HandleFunc("/hello", handle_hello).Methods("GET")

	http.ListenAndServe(":8080", router)
}

func handle_index(w http.ResponseWriter, r *http.Request) {
	w.Header().Set("ContentType", "text/html; charset=utf-8")
    
    tmpl, err := template.ParseFiles("./static/index.html")
    if err == nil {
        w.WriteHeader(http.StatusOK)
        context := Context{time.Now().String()}
        tmpl.Execute(w, context)
    }
    
}

func handle_hello(w http.ResponseWriter, r *http.Request) {
	w.Header().Set("ContentType", "text/plain; charset=utf-8")
	w.WriteHeader(http.StatusOK)
	w.Write([]byte("Hello and Welcome!!!"))
}