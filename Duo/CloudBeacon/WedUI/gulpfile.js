var gulp = require('gulp');
var concat = require('gulp-concat');
var server = require('gulp-server-livereload');



var paths = {
    scripts: ['./src/*.js'],
    html: ['./src/*.html']
};

gulp.task('default', ['scripts', 'html', 'watch', 'serve']);

gulp.task('scripts', function(done) {

    gulp.src(['./src/*.js'])
        .pipe(concat('app.js'))
        .pipe(gulp.dest('./www/js'))
        .on('error', function(error) {
            console.log(error);
            this.emit('end');
        })
        .on('end', done);

});

gulp.task('html', function(done) {

    gulp.src(['./src/*.html'])
        .pipe(gulp.dest('./www'))
        .on('error', function(error) {
            console.log(error);
            this.emit('end');
        })
        .on('end', done);;
});

gulp.task('watch', function() {

    gulp.watch(paths.scripts, ['scripts']);
    gulp.watch(paths.html, ['html']);
});

gulp.task('serve', function() {
  gulp.src('./www')
    .pipe(server({
      livereload: true,
      open: true
    }));
});

