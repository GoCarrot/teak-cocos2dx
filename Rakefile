# frozen_string_literal: true

require 'rake/clean'

CLEAN.include '**/.DS_Store'

namespace :build do
  task :android do
    sh 'cocos compile -p android'
  end
end

namespace :install do
  task :android do
    sh 'cocos run -p android'
  end
end
