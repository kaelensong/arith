# arith
compresses and decompresses a ppm image

//* * Architecture * *\\
Following the steps of chronologically of compression: 

(c) = compression
(d) = decompression

readwriteppm
        (c) reads the ppm image in for compression 
                - stores in Pnm_ppm
        (d) writes the ppm to stdout after it has been decompression

rgb
        (c) converts rgb to component-video, replaces rgb values with pb, pr
                and y values 
                - stores in Pnm_ppm
        (d) converts component-video to rgb
                - stores in Pnm_ppm

twobytwo
        (c) performs DCT and chroma transformations
                creates the 2x2 blocks and stores the a, b, c, d values and 
                average pb and pr values in the blocks
                - stores in UArray of block structs
        (d)  performs inverse DCT and chroma transformations
                converts a, b, c, d values back into y values, converts
                pb and pr to floats
                - stores in Pnm_ppm

packword
        (c) packs the average pb & pr, a, b, c, d 2x2 blocks into 32-bit words
                - stores in UArray of words
        (d) unpacks the 32-bit words and stores them in local variables (array)
                for further decompression
                - stores in UArray of block structs

readcompress
        (c) writes the compressed image to stdout with the specified header
        (d) reads in the file with the compressed image, creates a ppm
                with header information, reads and stores 32-bit words


