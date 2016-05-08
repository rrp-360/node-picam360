
raspivid -n -w 1080 -h 720 -fps 25 -vf -hf -t 86400000 -b 1800000 -ih -o - \
| ffmpeg -y \
    -i - \
    -c:v copy \
    -map 0:0 \
    -f ssegment \
    -segment_time 4 \
    -segment_format mpegts \
    -segment_list stream.m3u8 \
    -segment_list_size 1 \
    -segment_list_flags live \
    -segment_list_type m3u8 \
    "segments/%01d.ts" 
    