mkdir -p videos

ffmpeg -y -i raw.mp4 -ss 00:00:4.9 -to 00:00:9.6 -async 1 -strict -2 videos/ba.mp4
ffmpeg -y -i raw.mp4 -ss 00:00:14.5 -to 00:00:20 -async 1 -strict -2 videos/ga.mp4
ffmpeg -y -i raw.mp4 -ss 00:00:30.264 -to 00:00:34.735 -async 1 -strict -2 videos/va.mp4

# mkdir -p audios

# for v in videos/*; do
# 	ffmpeg -i $v -vn -ar 44100 -ac 2 -ab 192 -f mp3 `echo $v | sed 's/mp4/mp3/g' | sed 's/video/audio/g'`
# done

# mkdir -p mixed

# for v in videos/*; do
# 	for a in audios/*; do
# 		ffmpeg -i $v -i $a -c:v copy -map 0:v:0 -map 1:a:0 mixed/`echo $v | sed 's/videos\/\([a-z]*\).mp4/\1/g'`_`echo $a | sed 's/audios\/\([a-z]*\).mp3/\1/g'`.mp4
# 	done
# done
