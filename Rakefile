# frozen_string_literal: true

require 'rake/clean'
require 'fileutils'

CLEAN.include '**/.DS_Store'

PROJECT_PATH = Rake.application.original_dir

task :clean do
  sh 'git clean -fdx', verbose: false #unless ci?
end

namespace :package do
  task :copy do
    FileUtils.mkdir_p "#{PROJECT_PATH}/proj.android/app/libs"
    FileUtils.cp "#{PROJECT_PATH}/../teak-android/build/outputs/aar/teak-debug.aar", "#{PROJECT_PATH}/proj.android/app/libs/teak.aar"
  end

  task :build do
    Rake::Task['package:build:ios'].invoke
    Rake::Task['package:build:android'].invoke
    Rake::Task['package:copy'].invoke
  end

  namespace :build do
    task :ios do
      cd "#{PROJECT_PATH}/../teak-ios/", verbose: false do
        sh "BUILD_TYPE=Debug ./compile"
      end

      Rake::Task['package:copy'].invoke
    end

    task :android do
      cd "#{PROJECT_PATH}/../teak-android/", verbose: false do
        sh "./compile"
      end

      Rake::Task['package:copy'].invoke
    end
  end
end

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
