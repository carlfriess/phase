package main

import (
	"flag"
	"image"
	"image/color"
	_ "image/jpeg"
	"log"
	"os"
	"path/filepath"
	"text/template"
)

func RGB888(color color.Color) []uint8 {
	r, g, b, _ := color.RGBA()
	return []uint8{
		uint8(r >> 8),
		uint8(g >> 8),
		uint8(b >> 8),
	}
}

func RGB565(color color.Color) []uint8 {
	rgb := RGB888(color)
	rgb16 := ((uint16(rgb[0]) & 0b11111000) << 8) + ((uint16(rgb[1]) & 0b11111100) << 3) + (uint16(rgb[2]) >> 3)
	return []uint8{
		uint8(rgb16 >> 8),
		uint8(rgb16),
	}
}

func main() {

	var inputFile string
	var templateFile string
	var outputFile string

	flag.StringVar(&inputFile, "input", "", "Input image file")
	flag.StringVar(&templateFile, "template", "", "Template file for output")
	flag.StringVar(&outputFile, "output", "", "Output file")
	flag.Parse()

	// Open input image file
	input, err := os.Open(inputFile)
	if err != nil {
	    log.Fatal(err)
	}

	// Decode the image
	img, _, err := image.Decode(input)
	if err != nil {
		log.Fatal(err)
	}

	bounds := img.Bounds()
	size := bounds.Size()

	imgData := make([][]uint8, 0, size.X * size.Y)

	// Iterate each pixel and collect color data
	for y := bounds.Min.Y; y < bounds.Max.Y; y++ {
		for x := bounds.Min.X; x < bounds.Max.X; x++ {
			imgData = append(imgData, RGB888(img.At(x, y)))
		}
	}

	// Create the output file
	output, err := os.Create(outputFile)
	if err != nil {
		log.Fatalf("error creating output file: %v", err)
	}
	defer output.Close()

	// Determine the name of the image
	name := filepath.Base(outputFile)
	name = name[0:len(name) - len(filepath.Ext(name))]

	// Parse the template and execute it
	if t, err := template.ParseFiles(templateFile); err != nil {
		log.Fatalf("error parsing template file: %v", err)
	} else if err := t.Execute(output, struct {
		Name string
		Width int
		Height int
		Data [][]uint8
	}{
		Name: name,
		Width: size.X,
		Height: size.Y,
		Data: imgData,
	}); err != nil {
		log.Fatalf("error executing template: %v", err)
	}

}
