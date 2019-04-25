mkdir -p static dynamic3 dynamic2

video=ba
time=334

ffmpeg -y -i ${video}.mp4 -vf geq=0:128:128 black.mp4

ffmpeg -y -i black.mp4 -i ${video}.png \
	-filter_complex "[0:v][1:v] overlay=0:0:enable='between(t,0.$(( $time - 200 )),0.$(( $time + 100 )))'" \
	-pix_fmt yuv420p -c:a copy static/${video}.mp4

ffmpeg -y -i ${video}.mp4 -i black.png \
	-filter_complex "[0:v][1:v] overlay=0:0:enable='between(t,0.0,0.$(( $time )))'" \
	-pix_fmt yuv420p -c:a copy black.mp4
ffmpeg -y -i black.mp4 -i black.png \
	-filter_complex "[0:v][1:v] overlay=0:0:enable='between(t,0.$(( $time + 300 )),10.0)'" \
	-pix_fmt yuv420p -c:a copy dynamic3/${video}.mp4

ffmpeg -y -i dynamic3/${video}.mp4 -i black.png \
	-filter_complex "[0:v][1:v] overlay=0:0:enable='between(t,0.$(( $time + 100 )),0.$(( $time + 200 )))'" \
	-pix_fmt yuv420p -c:a copy dynamic2/${video}.mp4


video=ga
time=434

ffmpeg -y -i ${video}.mp4 -vf geq=0:128:128 black.mp4

ffmpeg -y -i black.mp4 -i ${video}.png \
	-filter_complex "[0:v][1:v] overlay=0:0:enable='between(t,0.$(( $time - 200 )),0.$(( $time + 100 )))'" \
	-pix_fmt yuv420p -c:a copy static/${video}.mp4

ffmpeg -y -i ${video}.mp4 -i black.png \
	-filter_complex "[0:v][1:v] overlay=0:0:enable='between(t,0.0,0.$(( $time )))'" \
	-pix_fmt yuv420p -c:a copy black.mp4
ffmpeg -y -i black.mp4 -i black.png \
	-filter_complex "[0:v][1:v] overlay=0:0:enable='between(t,0.$(( $time + 300 )),10.0)'" \
	-pix_fmt yuv420p -c:a copy dynamic3/${video}.mp4

ffmpeg -y -i dynamic3/${video}.mp4 -i black.png \
	-filter_complex "[0:v][1:v] overlay=0:0:enable='between(t,0.$(( $time + 100 )),0.$(( $time + 200 )))'" \
	-pix_fmt yuv420p -c:a copy dynamic2/${video}.mp4


video=va
time=234

ffmpeg -y -i ${video}.mp4 -vf geq=0:128:128 black.mp4

ffmpeg -y -i black.mp4 -i ${video}.png \
	-filter_complex "[0:v][1:v] overlay=0:0:enable='between(t,0.$(( $time - 100 )),0.$(( $time + 200 )))'" \
	-pix_fmt yuv420p -c:a copy static/${video}.mp4

ffmpeg -y -i ${video}.mp4 -i black.png \
	-filter_complex "[0:v][1:v] overlay=0:0:enable='between(t,0.0,0.$(( $time - 100 )))'" \
	-pix_fmt yuv420p -c:a copy black.mp4
ffmpeg -y -i black.mp4 -i black.png \
	-filter_complex "[0:v][1:v] overlay=0:0:enable='between(t,0.$(( $time + 200 )),10.0)'" \
	-pix_fmt yuv420p -c:a copy dynamic3/${video}.mp4

ffmpeg -y -i dynamic3/${video}.mp4 -i black.png \
	-filter_complex "[0:v][1:v] overlay=0:0:enable='between(t,0.$(( $time )),0.$(( $time + 100 )))'" \
	-pix_fmt yuv420p -c:a copy dynamic2/${video}.mp4

rm black.mp4
