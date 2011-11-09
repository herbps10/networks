#!/usr/bin/ruby

t = 0
while t < 1
	command = "/home/herb/git/networks/networks " + t.to_s + " " + (t + 0.001).to_s + " 0.001"

	exec command

	t += 0.001
	t = (t * 10000).round.to_f / 10000.0
end
