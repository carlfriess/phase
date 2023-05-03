package main

import (
	"flag"
	"image"
	"image/color"
	_ "image/jpeg"
	_ "image/png"
	"log"
	"os"
	"strings"
	"text/template"

	"github.com/marcinbor85/gohex"
)

type stringsValue []string

func (f *stringsValue) String() string {
	return strings.Join(*f, ", ")
}

func (f *stringsValue) Set(s string) error {
	*f = append(*f, strings.TrimSpace(s))
	return nil
}

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

func Mask8(color color.Color) []uint8 {
	_, _, _, a := color.RGBA()
	return []uint8{uint8(a >> 8)}
}

func loadImage(file string, mask bool) (data [][]uint8, size image.Point) {

	// Open input image file
	input, err := os.Open(file)
	if err != nil {
		log.Fatal(err)
	}

	// Decode the image
	img, _, err := image.Decode(input)
	if err != nil {
		log.Fatal(err)
	}

	bounds := img.Bounds()
	size = bounds.Size()

	imgData := make([][]uint8, 0, size.X*size.Y)

	// Iterate each pixel and collect color data
	for y := bounds.Min.Y; y < bounds.Max.Y; y++ {
		for x := bounds.Min.X; x < bounds.Max.X; x++ {
			if mask {
				imgData = append(imgData, Mask8(img.At(x, y)))
			} else {
				imgData = append(imgData, RGB888(img.At(x, y)))
			}
		}
	}

	return imgData, size
}

func embedSingleImage(name string, inputFile string, outputFile string, templateFile string, mask bool) {

	// Load image data
	imgData, size := loadImage(inputFile, mask)

	// Create the output file
	output, err := os.Create(outputFile)
	if err != nil {
		log.Fatalf("error creating output file: %v", err)
	}
	defer output.Close()

	// Parse the template and execute it
	if t, err := template.ParseFiles(templateFile); err != nil {
		log.Fatalf("error parsing template file: %v", err)
	} else if err := t.Execute(output, struct {
		Name   string
		Width  int
		Height int
		Data   [][]uint8
	}{
		Name:   name,
		Width:  size.X,
		Height: size.Y,
		Data:   imgData,
	}); err != nil {
		log.Fatalf("error executing template: %v", err)
	}

}

func main() {

	var simple bool
	var mask bool
	var hex bool
	var names stringsValue
	var inputFiles stringsValue
	var outputFiles stringsValue
	var templateFile string

	flag.BoolVar(&simple, "single", false, "Embed the input image in a C file")
	flag.BoolVar(&mask, "mask", false, "Treat the input image as a mask and embed in C file")
	flag.BoolVar(&hex, "hex", false, "Generate hex file containing a series of images")
	flag.Var(&names, "name", "Image name")
	flag.Var(&inputFiles, "input", "Input image file")
	flag.Var(&outputFiles, "output", "Output file")
	flag.StringVar(&templateFile, "template", "", "Template file for output")
	flag.Parse()

	if simple || mask {
		if len(names) != len(inputFiles) || len(inputFiles) != len(outputFiles) {
			log.Fatal("number of arguments does not match")
		}
		for i := 0; i < len(names); i++ {
			embedSingleImage(names[i], inputFiles[i], outputFiles[i], templateFile, mask)
		}
	} else if hex {
		if len(names) != len(inputFiles) {
			log.Fatal("number of arguments does not match")
		}

		var data []uint8
		meta := make(map[string]struct {
			Address int
			Width   int
			Height  int
		})

		// Iterate over inputs
		for i := 0; i < len(names); i++ {

			// Load image data
			var flatImgData []uint8
			imgData, size := loadImage(inputFiles[i], false)
			for _, pixel := range imgData {
				flatImgData = append(flatImgData, pixel...)
			}

			// Append image data
			meta[names[i]] = struct {
				Address int
				Width   int
				Height  int
			}{Address: 0x12000000 + len(data), Width: size.X, Height: size.Y}
			data = append(data, flatImgData...)

		}

		// Check total data size
		if len(data) > 0x1000000 {
			log.Fatal("Images do not fit in flash")
		}

		// Create output hex file
		hexFile, err := os.Create(outputFiles[0])
		if err != nil {
			log.Fatalf("error creating hex file: %v", err)
		}
		defer hexFile.Close()

		// Create new memory area
		mem := gohex.NewMemory()
		err = mem.AddBinary(0x12000000, data)
		if err != nil {
			log.Fatalf("error constructing hex file: %v", err)
		}
		err = mem.DumpIntelHex(hexFile, 16)
		if err != nil {
			log.Fatalf("error dumping hex file: %v", err)
		}

		// Create output index file
		indexFile, err := os.Create(outputFiles[1])
		if err != nil {
			log.Fatalf("error creating index file: %v", err)
		}
		defer indexFile.Close()

		// Parse the template and execute it
		if t, err := template.ParseFiles(templateFile); err != nil {
			log.Fatalf("error parsing template file: %v", err)
		} else if err := t.Execute(indexFile, meta); err != nil {
			log.Fatalf("error executing template: %v", err)
		}

	} else {
		log.Fatal("no operation specified")
	}

}
