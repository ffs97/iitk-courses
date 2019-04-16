from scipy.stats import mode

import torch
import torch.nn as nn
import torch.nn.functional as F

from torch.autograd import Variable


class AutoEncoderClassifier(nn.Module):
    def __init__(self, input_dim, n_channels, n_classes, kernel_size=3, stride=1, padding=1):
        super(AutoEncoderClassifier, self).__init__()

        self.n_classes = n_classes

        n_channels_enc = [input_dim] + n_channels + [n_classes]
        n_channels_dec = n_channels_enc[::-1]

        encoder = list()
        n_channels = n_channels_enc

        for i in range(len(n_channels) - 1):
            encoder.extend([
                nn.Conv2d(
                    n_channels[i], n_channels[i + 1],
                    kernel_size=kernel_size,
                    stride=stride, padding=padding
                ),
                nn.ReLU(),
                nn.BatchNorm2d(n_channels[i + 1])
            ])

        self.encoder = nn.Sequential(*encoder)

        decoder = list()
        n_channels = n_channels_dec

        for i in range(len(n_channels) - 1):
            decoder.extend([
                nn.Conv2d(
                    n_channels[i], n_channels[i + 1],
                    kernel_size=kernel_size,
                    stride=stride, padding=padding
                ),
                nn.ReLU(),
                nn.BatchNorm2d(n_channels[i + 1])
            ])

        self.decoder = nn.Sequential(*decoder)

        self.classification_loss_fn = nn.CrossEntropyLoss()

    def loss(self, y, y_, x, x_recon, blen=1, beta=1.0):
        recon_loss = -torch.mean(torch.sum(
            (torch.log(x_recon) * x).view(blen, -1), dim=1
        ))

        return self.classification_loss_fn(y, y_) + beta * recon_loss

    def step(self, x, sps, optimizer, beta=1.0, use_cuda=False):
        y, x_recon = self(x, True)

        y = y.permute(0, 2, 3, 1).contiguous()
        y = y.view(x.size(0), -1, self.n_classes)

        _, y_ = torch.max(y, dim=2)
        y_ = y_.data.cpu().numpy()

        for i in range(x.size(0)):
            for sp in sps[i]:
                y_[i][sp] = mode(y_[i][sp])[0]

        y_ = torch.from_numpy(y_)
        if use_cuda:
            y_ = y_.cuda()

        y = y.view(-1, self.n_classes)
        y_ = y_.view(-1)

        y_ = Variable(y_)

        loss = self.loss(y, y_, x, x_recon, x.size(0), beta)
        loss.backward()

        optimizer.zero_grad()
        optimizer.step()

        try:
            loss = loss.item()
        except:
            loss = loss.data[0]

        return loss

    def forward(self, x, recon_out=False):
        y = self.encoder(x)

        if recon_out:
            x_recon = self.decoder(y)
            x_recon = torch.sigmoid(x_recon)

            return y, x_recon

        else:
            return y
