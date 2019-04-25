mkdir -p full brief

ffmpeg -y -i ba.mp4 -ss 00:00:1.6 -to 00:00:03.2 -async 1 -strict -2 full/ba.mp4

ffmpeg -y -i full/ba.mp4 -ss 00:00:0.0 -to 00:00:01.0 -async 1 -strict -2 cut.mp4
ffmpeg -y -i cut.mp4 -filter:v "setpts=PTS/0.8" brief/ba.mp4


ffmpeg -y -i ga.mp4 -ss 00:00:2.302 -to 00:00:03.75 -async 1 -strict -2 full/ga.mp4

ffmpeg -y -i full/ga.mp4 -ss 00:00:0.0 -to 00:00:00.901 -async 1 -strict -2 cut.mp4
ffmpeg -y -i cut.mp4 -filter:v "setpts=PTS/0.8" brief/ga.mp4


ffmpeg -y -i va.mp4 -ss 00:00:1.635 -to 00:00:02.903 -async 1 -strict -2 full/va.mp4

ffmpeg -y -i full/va.mp4 -ss 00:00:0.0 -to 00:00:00.667 -async 1 -strict -2 cut.mp4
ffmpeg -y -i cut.mp4 -filter:v "setpts=PTS/0.8" brief/va.mp4

rm cut.mp4
