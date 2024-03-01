# arith
compresses and decompresses a ppm image <br />
<br />
# Architecture
Following the steps of chronologically of compression:  <br />
<br />
(c) = compression <br />
(d) = decompression <br />
<br />
readwriteppma <br />
        (c) reads the ppm image in for compression  <br />
                - stores in Pnm_ppm <br />
        (d) writes the ppm to stdout after it has been decompression <br />
<br />
rgb <br />
        (c) converts rgb to component-video, replaces rgb values with pb, pr and y values  <br />
                - stores in Pnm_ppm <br />
        (d) converts component-video to rgb <br />
                - stores in Pnm_ppm <br />
<br />
twobytwo <br />
        (c) performs DCT and chroma transformations <br />
                - creates the 2x2 blocks and stores the a, b, c, d values and average pb and pr values in the blocks <br />
                - stores in UArray of block structs <br />
        (d)  performs inverse DCT and chroma transformations <br />
                - converts a, b, c, d values back into y values, converts pb and pr to floats <br />
                - stores in Pnm_ppm <br />
<br />
packword <br />
        (c) packs the average pb & pr, a, b, c, d 2x2 blocks into 32-bit words <br />
                - stores in UArray of words <br />
        (d) unpacks the 32-bit words and stores them in local variables (array) for further decompression <br />
                - stores in UArray of block structs <br />
<br />
readcompress <br />
        (c) writes the compressed image to stdout with the specified header <br />
        (d) reads in the file with the compressed image, creates a ppm with header information, reads and stores 32-bit words <br />

