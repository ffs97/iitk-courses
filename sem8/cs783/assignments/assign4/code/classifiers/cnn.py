from scipy.stats import mode

import torch
import torch.nn as nn

from torch.autograd import Variable


class LinearCNNClassifier(nn.Module):
    def __init__(self, input_dim, n_channels, n_classes, kernel_size=3, stride=1, padding=1):
        super(LinearCNNClassifier, self).__init__()

        n_channels.insert(0, input_dim)

        self.n_classes = n_classes

        cnn = list()

        for i in range(len(n_channels) - 1):
            cnn.extend([
                nn.Conv2d(
                    n_channels[i], n_channels[i + 1],
                    kernel_size=kernel_size,
                    stride=stride, padding=padding
                ),
                nn.ReLU(),
                nn.BatchNorm2d(n_channels[i + 1])
            ])

        self.cnn = nn.Sequential(*cnn)

        self.classifier = nn.Sequential(
            nn.Conv2d(n_channels[-1], n_classes,
                      kernel_size=1, stride=1, padding=0),
            nn.BatchNorm2d(n_classes)
        )

        self.loss_fn = nn.CrossEntropyLoss()

    def loss(self, y, y_):
        return self.loss_fn(y, y_)

    def step(self, x, sps, optimizer, use_cuda):
        y = self(x)

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

        loss = self.loss(y, y_)
        loss.backward()

        optimizer.zero_grad()
        optimizer.step()

        try:
            loss = loss.item()
        except:
            loss = loss.data[0]

        return loss

    def forward(self, x):
        x = self.cnn(x)
        x = self.classifier(x)

        return x
