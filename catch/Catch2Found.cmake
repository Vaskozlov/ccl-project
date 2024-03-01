  enable_testing()

  file(
          GLOB_RECURSE
          CCL_CATCH_OBJECTS
          "catch/*.cpp"
  )

  add_executable(
          ccl-catch
          ${CCL_CATCH_OBJECTS}
  )

  isl_set_project_warnings(
          ccl-catch
          ${CCL_STRICT_COMPILATION}
          ""
          ""
          ""
          ""
          ${CCL_HARDENING}
          ${CCL_LIBCPP}
          OFF
  )

  add_dependencies(
          ccl-catch
          ccl
          fmt::fmt-header-only
          Catch2::Catch2
          Catch2::Catch2WithMain
  )

  target_link_libraries(
          ccl-catch
          ccl
          fmt::fmt-header-only
          Catch2::Catch2
          Catch2::Catch2WithMain
  )

  add_test(ccl-catch-test ccl-catch)