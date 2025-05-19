from diffusers import DiffusionPipeline
import torch

# HuggingFace'den örnek bir video diffusion modeli
model_id = "damo-vilab/text-to-video-ms-1.7b"

# Pipeline'ı yükle
pipe = DiffusionPipeline.from_pretrained(
    model_id, torch_dtype=torch.float16, variant="fp16"
)
pipe.to("cuda")  # GPU gerekiyorsa CUDA

# Metin girdisi
prompt = "a futuristic city with flying cars at sunset"

# Video üretimi
video_frames = pipe(prompt, num_inference_steps=25).frames

# Videoyu kaydet
video_path = "generated_video.mp4"

# ffmpeg ile video karelerini mp4'e dönüştür
import imageio
imageio.mimsave(video_path, video_frames, fps=8)

print(f"Video başarıyla üretildi: {video_path}")
